#include "qqjson.h"

const protocol_tuple sla_fields[] = {
	{"version", INTEGER_TYPE},
	{"sla_version", INTEGER_TYPE},
	{"client_id", STRING_TYPE},
	{"storage_id", ARRAY_TYPE,
		{
			{"s_dev", INTEGER_TYPE},
			{"i_ino", INTEGER_TYPE}
		}
	},
	{"storage_type", INTEGER_TYPE},
	{"iops_max", INTEGER_TYPE},
	{"iops_min", INTEGER_TYPE},
	{"throughput_min", INTEGER_TYPE},
	{"throughput_max", INTEGER_TYPE},
	{"unused_bandwidth", INTEGER_TYPE}
};

const protocol_tuple feedback_fields[] = {
	{"version", INTEGER_TYPE},
	{"sla_version", INTEGER_TYPE},
	{"client_id", STRING_TYPE},
	{"storage_id", ARRAY_TYPE,
		{
			{"s_dev", INTEGER_TYPE},
			{"i_ino", INTEGER_TYPE}
		}
	},
	{"storage_type", INTEGER_TYPE},
	{"current_throughput", INTEGER_TYPE},
	{"writes_queued", INTEGER_TYPE},
	{"reads_queued", INTEGER_TYPE},
	{"suspensions", INTEGER_TYPE},
	{"sla_check", INTEGER_TYPE},
	{"critical_request", INTEGER_TYPE}
};

/** SLA PARSING **/

void *qq_load_sla(char *sla)
{
	json_error_t error;
	json_t *curr_sla = json_loads(sla, 0, &error);
	free(sla);
	if (!curr_sla) {
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return NULL;
	}
	return curr_sla;
}

int qq_validate_sla(json_t *curr_sla)
{
	if (!json_is_object(curr_sla))
		return 0;
	int sla_length = sizeof(sla_fields) / sizeof(protocol_tuple);
	int errors = 0;
	for (int i = 0; i < sla_length; i++)
		errors += qq_test_tuple(curr_sla, sla_fields[i]);
	return errors;
}

int qq_release_sla(json_t *curr_sla)
{
	json_decref(curr_sla);
	return 0;
}

/** CLIENT FEEDBACK CONSTRUCTION **/

/**
 * Construct client feedback JSON object from client_feedback
 * input struct.
 */
void *qq_construct_client_feedback(client_feedback *cf)
{
	json_t *cf_json = json_pack("{sisis[sisi]sisisisisisisi}",
		"version", cf->version,
		"sla_version", cf->sla_version,
		"storage_id",
			"s_dev", cf->storage_id.s_dev,
			"i_ino", cf->storage_id.i_ino,
		"storage_type", cf->storage_type,
		"current_throughput", cf->current_throughput,
		"writes_queued", cf->writes_queued,
		"reads_queued", cf->reads_queued,
		"suspensions", cf->suspensions,
		"sla_check", cf->sla_check,
		"critical_request", cf->critical_request
	);
	return cf_json;
}

char *qq_obj_to_string(json_t *obj) {
	return json_dumps(obj, 0);
}

/** GENERAL PARSING FUNCTIONS **/

/**
 * Test an SLA for validity in accordance with the supplied tuple.
 * The tuple contains the name and type of information expected.
 * If the SLA field is not present or not of the expected type...
 * Return errors.
 * Return 0 means SLA information is valid.
 * Return > 0 means at least 1 error is present.
 */
int qq_test_tuple(json_t *curr_sla, protocol_tuple tuple)
{
	char *sla_field_name = tuple.name;
	int sla_field_type = tuple.type;
	int errors = 0;
	if (sla_field_type == STRING_TYPE)
		errors += qq_test_string(curr_sla, sla_field_name);
	else if (sla_field_type == INTEGER_TYPE)
		errors += qq_test_integer(curr_sla, sla_field_name);
	else if (sla_field_type == ARRAY_TYPE) {
		int is_array = qq_test_array(curr_sla, sla_field_name);
		if (!is_array) {
			errors += 1;
		} else {
			protocol_tuple *sub_tuples = tuple.sub_tuples;
			errors += qq_test_array_tuples(curr_sla, sla_field_name, sub_tuples);
		}
	}
	return errors;
}

/**
 * Test whether or not a JSON object by a name is an integer.
 * Return whether or not it is an integer. (0, 1)
 */
int qq_test_integer(json_t *curr_sla, char *item)
{
	json_t *test_obj = json_object_get(curr_sla, item);
	int result = 0;
	if (!json_is_integer(test_obj))
		result++;
	json_decref(test_obj);
	return result;
}

/**
 * Test whether or not a JSON object by a name is a string.
 * Return whether or not it is a string. (0, 1)
 */
int qq_test_string(json_t *curr_sla, char *item)
{
	json_t *test_obj = json_object_get(curr_sla, item);
	int result = 0;
	if (!json_is_string(test_obj))
		result++;
	json_decref(test_obj);
	return result;
}

/**
 * Test whether or not a JSON object by a name is an array.
 * Return whether or not it is an array. (0, 1)
 */
int qq_test_array(json_t *curr_sla, char *item) {
	json_t *test_obj = json_object_get(curr_sla, item);
	int result = 0;
	if (!json_is_array(test_obj))
		result++;
	json_decref(test_obj);
	return result;
}

/**
 * Test the tuples inside of an existing array.
 * Requires array elements to be in protocol_tuple format.
 * Return number of errors encountered.
 */
int qq_test_array_tuples(json_t *curr_sla, char *item, protocol_tuple *sub_tuples) {
	int errors = 0;
	if (qq_test_array(curr_sla, item)) {
		int length = sizeof(sub_tuples) / sizeof(protocol_tuple);
		json_t *array = json_object_get(curr_sla, item);
		for (int i = 0; i < length; i++)
			errors += qq_test_tuple(array, sub_tuples[i]);
		json_decref(array);
	} else
		errors++;
	return errors;
}