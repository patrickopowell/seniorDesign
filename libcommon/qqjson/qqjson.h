#ifndef JANSSON_INCLUDE
#include <jansson.h>
#include "../protocol.h"
#include "../logging/logging.h"

#define STRING_TYPE 0
#define INTEGER_TYPE 1
#define ARRAY_TYPE 2

#define QQJSON_VALID 0
#define QQJSON_NOT_JSON -1
#define QQJSON_NOT_VALID -2

struct protocol_tuple {
	char *name;
	int type;
	protocol_tuple *sub_tuples;
};

/** SLA PARSING **/
void *qq_load_sla(char *);
int qq_validate_sla(json_t *);
int qq_release_sla(json_t *);

/** GENERAL PARSING FUNCTIONS **/
int qq_test_tuple(json_t *curr_sla, protocol_tuple tuple);
int qq_test_integer(json_t *, char *);
int qq_test_string(json_t *, char *);
int qq_test_array(json_t *, char *);
int qq_test_array_tuples(json_t *curr_sla, char *item, protocol_tuple *sub_tuples);
#define JANSSON_INCLUDE
#endif