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
* @author Patrick Powell
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
	
	printf("\nrb_rate = %d\n", rb_ptr->rb_rate);
        
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
* @author Patrick Powell
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
	if (rb_ptr->rb_tokens > 0) {
		rb_ptr->rb_tokens--;
		return 1;
	}

	return 0;
}


/**
*
* Storage QoS throttle function - Throttling function.  Checks for interrupted call.  Will return when caller can proceed.
*
* @author Patrick Powell
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


/**
*
* Storage QoS increment queue function -   increments request type count.
*
* @author Patrick Powell
* 
* @return void
*
*/

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
* @author Patrick Powell
* 
* @return void
*
*/

int get_bucket(const char *path)
{
	int pos = 0;
	
	com_lock_sla();
	
	if (strcmp(com_sla_list->slas[0].path, "") == 0) {
		com_unlock_sla();
		
		qq_log_critical("Could not release lock file! Fatal error.");
		qq_log_debug(strerror(errno));
		
		return -1;
	}
	
	while (pos<COM_MAX_SERVERS && strcmp( com_sla_list->slas[pos].path, path ) != 0 )
    {
		pos++;
	}
		
	if(pos == COM_MAX_SERVERS-1 && strcmp( com_sla_list->slas[pos].path, path ) != 0) {
		com_unlock_sla();
		
		qq_log_critical("Could not release lock file! Fatal error.");
		qq_log_debug(strerror(errno));
		
		return -1;
	}
	
	if (strcmp( rb_mounts[pos].rb_path, path ) != 0) add_bucket(path, pos, com_sla_list->slas[pos].iops_max);
	
	rb = rb_mounts[pos];
	
	strcpy(rb.rb_path, path);
	rb.rb_rate = com_sla_list->slas[pos].iops_max;
	// cap tokens at 10% of rate
	rb.rb_token_cap = rb.rb_rate / 10;
	
	com_unlock_sla();
	
	return pos;
}

/**
*
* Add token bucket for specific mountpoint
*
* @author Patrick Powell
* 
* @return void
*
*/

void add_bucket(const char *path, unsigned int index, unsigned int rate)
{
	int pos = 0;
	
	qq_log_info("Adding token bucket.");
	
	while ((pos<COM_MAX_SERVERS && strcmp( com_sla_list->slas[pos].path, path ) != 0) || strcmp( com_sla_list->slas[pos].path, "" ) != 0 )
    pos++;
	
	strcpy(rb_mounts[pos].rb_path, path);
	rb_mounts[pos].rb_rate = com_sla_list->slas[pos].iops_max;
	rb_mounts[pos].rb_token_cap = rate / 10;
	rb_mounts[pos].rb_tokens = rate / 10;
}

/**
*
* Get time since system boot
*
* @author Patrick Powell
* 
* @return long system uptime
*
*/

unsigned long qos_get_uptime(void)
{
    struct timespec t_info;
    clock_gettime(CLOCK_REALTIME, &t_info);
	
	unsigned long seconds;

	// remove 30 years from time index
	seconds = SEC_PER_YEAR * 30;
	
	seconds = t_info.tv_sec - seconds;
	
	unsigned long microseconds = (seconds*MSEC_PER_SEC) + (t_info.tv_nsec/1000);
    
    return microseconds;
}

/**
*
* Main function
*
* @author Patrick Powell
* 
* @return 1 when finished
*
*/

int qos_init(const char *path) 
{
	com_init_mem();
	
	get_bucket(path);
	
	qq_log_info("Initializing QoS throttling.");
	
	return 1;
}

/**
*
* Release function - release shared memory, called when qqfs unmounts
*
* @author Patrick Powell
* 
* @return 1 when finished
*
*/

int qos_release(void)
{
	com_close_mem();
	
	qq_log_info("Releasing QoS throttling.");
	
	return 1;
}

