/**
*
* Includes
*
*/

#include <stdio.h>
#include <time.h>
#include <sys/sysinfo.h>


#define SEC_PER_YEAR 31557600
#define MSEC_PER_SEC 1000000

typedef struct ratebucket {
	// generic ID to use when you have multiple ratebuckets
	unsigned int rb_id; 
	// Rate at which tokens are generated per second
	unsigned int rb_rate; 
	// Think of this as the number of requests that can be handle "right now"
	unsigned int rb_tokens; 
	// Depth of bucket. i.e. maximum number of tokens that can accumulate
 	unsigned int rb_token_cap; 
	// Timestamp when tokens were last updated. I recommend microsecond granularity  
	unsigned long rb_ts; 
} ratebucket_t;

void qos_throttle (void);
void update_token (struct ratebucket *rb_ptr);
int qos_can_send (struct ratebucket *rb_ptr);
static unsigned long qos_get_uptime(void);

//struct qos_monitor monitor;

ratebucket_t rb;