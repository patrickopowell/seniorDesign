#ifndef JANSSON_INCLUDE
#include <jansson.h>

#define STRING_TYPE 0
#define INTEGER_TYPE 1
#define ARRAY_TYPE 2

typedef struct protocol_tuple protocol_tuple;

struct protocol_tuple {
	char *name;
	int type;
	protocol_tuple *sub_tuples;
};

typedef client_feedback client_feedback;

struct client_feedback {
	int version;
	int sla_version;
	struct storage_id {
		int s_dev;
		int i_ino;
	};
	int storage_type;
	int current_throughput;
	int writes_queued;
	int reads_queued;
	int suspensions;
	int sla_check;
	int critical_request;
};

typedef sla sla;

struct sla {
	int version;
	int sla_version;
	char *client_id;
	struct storage_id {
		int s_dev;
		int i_ino;
	};
	int storage_type;
	int iops_max;
	int iops_min;
	int throughput_min;
	int throughput_max;
	int unused_bandwidth;
};

/** SLA PARSING **/
void *qos_load_sla(char *);
int qos_validate_sla(json_t *);
int qos_release_sla(json_t *);

/** GENERAL PARSING FUNCTIONS **/
int qos_test_tuple(json_t *curr_sla, protocol_tuple tuple);
int qos_test_integer(json_t *, char *);
int qos_test_string(json_t *, char *);
int qos_test_array(json_t *, char *);
int qos_test_array_tuples(json_t *curr_sla, char *item, protocol_tuple *sub_tuples);
#define JANSSON_INCLUDE
#endif