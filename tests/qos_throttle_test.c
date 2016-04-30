#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdio.h>

#include "../qqfs/src/qos_throttle.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

ratebucket_t rb;

char *spacer = "";

void test_token_bucket(void)
{
    rb.rb_ts = qos_get_uptime();
	
	rb.rb_rate = 200000; // replace with value passed through control
	
    rb.rb_tokens = 0;
	
    rb.rb_token_cap = 20000; // 10 percent of rate. Controls size of bursts
	
	unsigned int current_ts = qos_get_uptime();

	unsigned int time_diff = current_ts - rb.rb_ts;
	
	printf("\n%40stest_token_bucket() - time_diff = %lu\n", spacer, time_diff);
	
	CU_ASSERT(time_diff < 1000000);
	CU_ASSERT(qos_can_send(&rb) == 0);
	
	printf("\n%40stest_token_bucket() - tokens = %lu\n", spacer, rb.rb_tokens);
}

void test_throttle(void)
{
	rb.rb_rate = 200000; // replace with value passed through control
	
    rb.rb_tokens = 0;
	
    rb.rb_token_cap = 20000; // 10 percent of rate. Controls size of bursts
	
    rb.rb_ts = qos_get_uptime();
	
	printf("\n%40stest_throttle() - rb_ts = %lu\n", spacer, rb.rb_ts);

	qos_throttle("/home/vagrant/QualiQueue/2016springTeam28/qqfs/example/mountdir/",1);

	unsigned int uptime = qos_get_uptime() - rb.rb_ts;
	
	printf("%40stest_throttle() - uptime = %lu\n", spacer, uptime);

	CU_ASSERT(uptime > 900000 && uptime < 1100000);
}

void test_update_tokens(void)
{
	rb.rb_rate = 200000; // replace with value passed through control
	
    rb.rb_tokens = 0;
	
    rb.rb_token_cap = 20000; // 10 percent of rate. Controls size of bursts
	
    rb.rb_ts = qos_get_uptime();
	
	printf("\n%40stest_update_tokens() - rb_ts = %lu\n", spacer, rb.rb_ts);

	update_tokens(&rb);
	
	printf("\n%40stest_update_tokens() - tokens = %lu\n", spacer, rb.rb_tokens);

	CU_ASSERT(rb.rb_tokens > 0);
}

int main(void)
{
	CU_pSuite pSuite = NULL;
	
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();
	
	pSuite = CU_add_suite("qos_throttle_test", init_suite, clean_suite);
	
	if ( (NULL == CU_add_test(pSuite, "test_token_bucket", test_token_bucket)) ||
		(NULL == CU_add_test(pSuite, "test_update_tokens", test_update_tokens)) ||
		(NULL == CU_add_test(pSuite, "test_throttle", test_throttle))
	) {
		CU_cleanup_registry();
		return CU_get_error();		
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");
	
	CU_cleanup_registry();
	
	return CU_get_error();

}