/**
*
* Includes
*
*/

#include <stdio.h>
#include <time.h>
#include <sys/sysinfo.h>

#include "../../libcommon/client/communication.h"


#define SEC_PER_YEAR 31557600
#define MSEC_PER_SEC 1000000

#define QOS_READ_OPS 1
#define QOS_WRITE_OPS 2

typedef struct ratebucket {
	// generic ID to use when you have multiple ratebuckets
	unsigned int rb_id; 
	// ratebucket mountpoint path
	char rb_path[80];
	// Rate at which tokens are generated per second
	unsigned int rb_rate; 
	// Think of this as the number of requests that can be handle "right now"
	unsigned int rb_tokens; 
	// Depth of bucket. i.e. maximum number of tokens that can accumulate
 	unsigned int rb_token_cap; 
	// Timestamp when tokens were last updated. I recommend microsecond granularity  
	unsigned long rb_ts; 
} ratebucket_t;

typedef struct
{
	unsigned int bytes_read;
	unsigned int bytes_written;
	
	unsigned int read_ops;
	unsigned int write_ops;
	
	unsigned int reads_queued;
	unsigned int writes_queued;
	
	unsigned int suspensions;
} qos_monitor;

void qos_throttle (const char *path, int req);
void inc_queue(int index, int req);
int get_bucket(const char *path);
void add_bucket(const char *path, unsigned int index, unsigned int rate);
void update_token (struct ratebucket *rb_ptr);
int qos_can_send (struct ratebucket *rb_ptr);
unsigned long qos_get_uptime(void);
int qos_init(const char *path);
int qos_release(void);

qos_monitor monitor;

ratebucket_t rb;

ratebucket_t rb_mounts[5];
