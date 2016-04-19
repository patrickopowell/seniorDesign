#include <CUnit/CUnit.h>

ratebucket_t rb;

	void test_token_bucket(void)
{
	rb.rb_rate = 2000; // replace with value passed through control
	
    rb.rb_tokens = 0;
	
    rb.rb_token_cap = 200; // 10 percent of rate. Controls size of bursts
	
    rb.rb_ts = qos_get_uptime();

	unsigned int time_diff = current_ts - rb_ptr->rb_ts;

	CU_ASSERT(qos_can_send(&rb) == 0);
}

void test_throttle(void)
{
	rb.rb_rate = 2000; // replace with value passed through control
	
    rb.rb_tokens = 0;
	
    rb.rb_token_cap = 200; // 10 percent of rate. Controls size of bursts
	
    rb.rb_ts = qos_get_uptime();

	qos_throttle(1,1);

	unsigned int uptime = qos_get_uptime() - rb.rb_ts;

	if (uptime > 900000 || uptime < 1100000)
		CU_PASS("qos_throttle() waited 1 second\n");

	else
		CU_FAIL("qos_throttle() waited too long\n");
}