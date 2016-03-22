#include "qos-json.h"

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

int qos_load_sla(char *sla)
{
	if (sla_loaded > 0)
		release_sla();
	json_error_t error;
	json_t *curr_sla = json_loads(sla, 0, &error);
	free(sla);
	if (!curr_sla) {
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return NULL;
	}
	return curr_sla;
}

int qos_validate_sla(json_t *curr_sla)
{
	if (!json_is_object(curr_sla))
		return 0;
	int sla_length = sizeof(sla_fields) / sizeof(protocol_tuple);
	int errors = 0;
	for (int i = 0; i < sla_length; i++) {
		char *sla_field_name = sla_fields[i].name;
		int sla_field_type = sla_fields[i].type;
		if (sla_field_type == STRING_TYPE)
			errors += qos_test_string(curr_sla, sla_field_name);
		else if (sla_field_type == INTEGER_TYPE)
			errors += qos_test_integer(curr_sla, sla_field_name);
		else if (sla_field_type == ARRAY_TYPE) {
			is_array = qos_test_array(curr_sla, sla_field_name);
			if (!is_array) {
				errors += 1;
				continue;
			}
			protocol_tuple sub_tuples[] = sla_fields[i].sub_tuples;
		}
	}
	return errors;
}

int qos_validate_tuple(protocol_tuple *tuple) {

}

int qos_release_sla(json_t *curr_sla)
{
	json_decref(curr_sla);
	return 0;
}

/** CLIENT FEEDBACK CONSTRUCTION **/



/** GENERAL PARSING FUNCTIONS **/

int qos_test_integer(json_t *curr_sla, char *item)
{
	test_obj = json_object_get(curr_sla, item);
	int result = json_is_integer(test_obj);
	json_decref(test_obj);
	return result;
}

int qos_test_string(json_t *curr_sla, char *item)
{
	test_obj = json_object_get(curr_sla, item);
	int result = json_is_string(test_obj);
	json_decref(test_obj);
	return result;
}

int qos_test_array(json_t *curr_sla, char *item) {
	test_obj = json_object_get(curr_sla, item);
	int result = json_is_array(test_obj);
	json_decref(test_obj);
	return result;
}