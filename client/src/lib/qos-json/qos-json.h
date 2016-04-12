#include <jansson.h>
#include <stdio.h>

#define STRING_TYPE 0
#define INTEGER_TYPE 1
#define ARRAY_TYPE 2

struct protocol_tuple {
	char *name;
	int type;
	protocol_tuple sub_tuples[];
};

/** SLA PARSING **/
int qos_load_sla(char *);
int qos_validate_sla(json_t *);
int qos_release_sla(json_t *);

/** GENERAL PARSING FUNCTIONS **/
int qos_test_tuple(json_t *, protocol_tuple *);
int qos_test_integer(json_t *, char *);
int qos_test_string(json_t *, char *);
int qos_test_array(json_t *, char *);
int qos_test_array_tuples(json_t *, protocol_tuple *);