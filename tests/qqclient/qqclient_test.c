#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>

#include "main.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

char *spacer = "";

void test_token_bucket(void)
{
    
	
	CU_ASSERT(time_diff < 1000000);
	CU_ASSERT(qos_can_send(&rb) == 0);
}

void test_throttle(void)
{
	

	CU_ASSERT(uptime > 900000 && uptime < 2000000);

	CU_ASSERT(uptime > 900000 && uptime < 2000000);
}

void test_update_tokens(void)
{
	

	CU_ASSERT(rb.rb_tokens <= 20000);
	
}

void test_init(void)
{
	CU_ASSERT(qos_init("/home/vagrant/QualiQueue/2016springTeam28/qqfs/example/mountdir/") == 1);
	CU_ASSERT(qos_release() == 1);
}

int main(void)
{
	com_init_mem();
	
	strcpy(com_stat_list->stats[0].path, "/home/vagrant/QualiQueue/2016springTeam28/qqfs/example/mountdir/");
	com_stat_list->stats[0].iops_sec = 20000;
	
	strcpy(com_sla_list->slas[0].path, "/home/vagrant/QualiQueue/2016springTeam28/qqfs/example/mountdir/");
	com_sla_list->slas[0].iops_max = 20000;
	
	CU_pSuite pSuite = NULL;
	
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();
	
	pSuite = CU_add_suite("qqclient_test", init_suite, clean_suite);
	
	if ( (NULL == CU_add_test(pSuite, "test_token_bucket", test_token_bucket)) ||
		(NULL == CU_add_test(pSuite, "test_update_tokens", test_update_tokens)) ||
		(NULL == CU_add_test(pSuite, "test_throttle", test_throttle)) ||
		(NULL == CU_add_test(pSuite, "test_init", test_init))
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
	
	com_close_mem();
	
	return CU_get_error();

}
