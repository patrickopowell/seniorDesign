/*

	QualiQueue File System (QQFS)
	
	This program is a modification to the Big Brother File System (BBFS).
	This code and its included files implement the storage Quality-of-Service
	functionality to paths mounted in the Quality-of-Service File System.

*/

#include "qos_throttle.h"

/**
*
* Storage QoS update tokens function - called from kernel space when out of tokens
*
* @return void
*
*/

void update_tokens (struct ratebucket *rb_ptr)
{
unsigned int current_ts = qos_get_uptime();
unsigned int time_diff;
unsigned int tokens;

	time_diff = current_ts - rb_ptr->rb_ts;
        
	// This check here prevents overflow if too much time has passed
	if (time_diff > 1000000) { // More than 1 second has passed
		tokens = rb_ptr->rb_rate;
	} else {
		// time_diff is in usecs. Rate is in ops per second.
		tokens = (time_diff * rb_ptr->rb_rate)/1000000;
	}
	
	if (tokens > 0) {
		rb_ptr->rb_tokens += tokens;
		// Check credit, do not allow it to exceed cap. 
		if (rb_ptr->rb_tokens > rb_ptr->rb_token_cap) {
			rb_ptr->rb_tokens = rb_ptr->rb_token_cap;
			rb_ptr->rb_ts = current_ts;
		} else {
			// This is a tricky part. Due to rounding, we do not get the full credit we are due.
			// Advance the ratebucket timestamp based on the number of tokens generated.
			rb_ptr->rb_ts += (tokens * 1000000)/rb_ptr->rb_rate;
		}
	}
	// overflow safety check
	if (rb_ptr->rb_tokens > rb_ptr->rb_token_cap) {
		rb_ptr->rb_tokens = rb_ptr->rb_token_cap;
	}	
}

/**
*
* Storage QoS can send function - called from kernel space to verify token availability
*
* @return bool true if request can be sent
*
*/

int qos_can_send (struct ratebucket *rb_ptr)
{

	if (rb_ptr->rb_tokens > 0) {
		rb_ptr->rb_tokens--;
		return 1;
	}
	// Out of tokens. Update ratebucket and try again
	update_tokens(rb_ptr);

	return 0;
}


// For completeness, will check for interrupted call.
/**
*
* Storage QoS throttle function - Throttling function. Will return when caller can proceed.
*
* @return void
*
*/


void qos_throttle (const char *path, int req)
{
	
	int index = get_bucket(path);
	
	if (index < 0) return;
	
	while(!qos_can_send(&rb)) {
		
		sleep(1);
		
		inc_queue(index, req);
	}
	return;
}

void inc_queue(int index, int req)
{
	com_lock_stat();
	
	switch(req)
	{
		// Read operations
		case QOS_READ_OPS:
			monitor.reads_queued++;
			com_stat_list->stats[index].reads_queued++;
			break;
		// Write operations
		case QOS_WRITE_OPS:
			monitor.writes_queued++;
			com_stat_list->stats[index].writes_queued++;
			break;
	}
	
	monitor.suspensions++;
	com_stat_list->stats[index].iops_suspended++;
	
	com_unlock_stat();
	
}

/**
*
* Get token bucket for specific mountpoint
*
* @return void
*
*/

int get_bucket(const char *path)
{
	int pos = 0;
	
	com_lock_sla();
	
	while (pos<5 && strcmp( com_sla_list->slas[pos].path, path ) != 0 )
    {
		printf("\n---sla[%d] = %s (%d)\n", pos, path, strlen(path));
		printf("---com_sla_list->slas[%d] = %s (%d)\n", pos, com_sla_list->slas[pos].path, strlen(path));
		pos++;
	}
	
	printf("\n---original_path = %s (%d)\n", pos, path, strlen(path));
	printf("---rb_mounts[%d].path = %s (%d)\n", pos, rb_mounts[pos].rb_path, strlen(rb_mounts[pos].rb_path));
	printf("---com_sla_list->slas[%d].path = %s (%d)\n", pos, com_sla_list->slas[pos].path, strlen(com_sla_list->slas[pos].path));
		
	if(pos == 4 && strcmp( com_sla_list->slas[pos].path, path ) != 0) return -1;
	
	if (strcmp( rb_mounts[pos].rb_path, path ) != 0) add_bucket(path, pos, com_sla_list->slas[pos].iops_max);
	
	rb = rb_mounts[pos];
	
	strcpy(rb.rb_path, path);
	rb.rb_rate = com_sla_list->slas[pos].iops_max;
	rb.rb_token_cap = rb.rb_rate / 10;
	
	com_unlock_sla();
	
	return pos;
}

/**
*
* Add token bucket for specific mountpoint
*
* @return void
*
*/

void add_bucket(const char *path, unsigned int index, unsigned int rate)
{
	int pos = 0;
	
	while ((pos<5 && strcmp( com_sla_list->slas[pos].path, path ) != 0) || strcmp( com_sla_list->slas[pos].path, "" ) != 0 )
    pos++;

	printf("\n---add_bucket() - pos = %d, index = %d\n", pos, index);
	
	strcpy(rb_mounts[pos].rb_path, path);
	rb_mounts[pos].rb_rate = com_sla_list->slas[pos].iops_max;
	rb_mounts[pos].rb_token_cap = rate / 10;
	rb_mounts[pos].rb_tokens = rate / 10;
}

/**
*
* Get time since system boot
*
* @return long system uptime
*
*/

unsigned long qos_get_uptime(void)
{
    struct timespec t_info;
    clock_gettime(CLOCK_REALTIME, &t_info);
	
	unsigned long seconds;

	seconds = SEC_PER_YEAR * 30;
	
	seconds = t_info.tv_sec - seconds;
	
	unsigned long microseconds = (seconds*MSEC_PER_SEC) + (t_info.tv_nsec/1000);
    
    return microseconds;
}

/**
*
* Main function
*
* @return 1 when finished
*
*/

int qos_init(const char *path) 
{
	printf("\ninit_mem");
	com_init_mem();
	printf("\nget_bucket");
	get_bucket(path);
	printf("\nreturn\n");
	return 1;
}

/**
*
* Release function - release shared memory, called when qqfs unmounts
*
* @return 1 when finished
*
*/

int qos_release(void)
{
	com_close_mem();
	
	return 1;
}

