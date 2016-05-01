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
	/*if (rb_ptr->rb_tokens > 0) {
		rb_ptr->rb_tokens--;
		return 1;
	}*/

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

//void throttle(request_t *req)
void qos_throttle (const char *path, int req)
{
	
	if(!get_bucket(path)) return;//rb.rb_id = mountID; iterate through buckets to verify the right rate limit
	
	while(!qos_can_send(&rb)) {
		//struct timespec ts, ts2;
		//ts.tv_nsec = 1000;
		
		sleep(1);
		
		inc_queue(req);
		
		/*if( nanosleep(&ts,&ts2) < 0 ) {
			printf("sleep failed\n");
		}*/ // Some sleep function. Linux has lots to choose from.
		//if (interrupted()) { // In case user got impatient. Some Linux function that checks process state.
		//	return;
		//}
	}
	return;
}

void inc_queue(int req)
{
	
	switch(req)
	{
		// Read operations
		case QOS_READ_OPS:
			monitor.reads_queued++;
			
			break;
		// Write operations
		case QOS_WRITE_OPS:
			monitor.writes_queued++;
			
			break;
	}
	
	monitor.suspensions++;
	
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
	#if UNSAFE
	shr_lock_sla();
	#endif
	while (pos<5 && strcmp( shr_stat_list->stats[pos]->path, path ) != 0 )
    pos++;

	if(pos == 4 && strcmp( shr_stat_list->stats[pos]->path, path ) != 0) return 0;
	
	if (strcmp( rb_mounts[pos]->, "" ) != 0) add_bucket(path, pos, shr_stat_list->stats[pos]->iops_sec);
	
	rb = rb_mounts[pos];
	
	rb.rb_path = path[pos];
	rb.rb_rate = shr_stat_list->stats[pos]->iops_sec;
	rb.rb_token_cap = rb_rate / 10;
	

	#if UNSAFE
	shr_unlock_sla();
	#endif
	return 1;
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
	#if UNSAFE
	shr_lock_sla();
	#endif
	while (pos<5 && strcmp( shr_stat_list->stats[pos]->path, path ) != 0 || strcmp( shr_stat_list->stats[pos]->path, "" ) != 0 )
    pos++;
	
	rb_mounts[pos].rb_path = path[pos];
	rb_mounts[pos].rb_rate = shr_stat_list->stats[pos]->iops_sec;
	rb_mounts[pos].rb_token_cap = rate / 10;
	rb_mounts[pos].rb_tokens = rate / 10;
	rb_mounts[pos].rb_ts = qos_get_uptime();
	

	#if UNSAFE
	shr_unlock_sla();
	#endif
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
* 
*
*/

int qos_init(const char *path) 
{
	shr_init_mem();
	
	//rb.rb_rate = 2000; // replace with value passed through control
	
    //rb.rb_tokens = 200; // 10 percent of rate. ~100ms of iops at rate/second
	
    //rb.rb_token_cap = 200; // 10 percent of rate. Controls size of bursts
	
    //rb.rb_ts = qos_get_uptime();
	
	get_bucket(path);
	
	shr_close_mem();
	
	return 1;
}

