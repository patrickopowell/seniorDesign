#include "sla.h"

int sla_loaded = 0;
json_t *curr_sla;
json_error_t error;

int load_sla(char *sla) {
	if (sla_loaded > 0)
		release_sla();
	curr_sla = json_loads(sla, 0, &error);
	free(sla);
	if (!curr_sla) {
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}
	sla_loaded = 1;
	return 0;
}

int release_sla() {
	if (sla_loaded == 0)
		return 1;
	json_decref(curr_sla);
	sla_loaded = 0;
	return 0;
}

/**
 * Validates an SLA in its entirety across all protocol fields.
 * Needs to be refactored with:
 * - Individual methods
 * - Loggable messages
 * - Easy error passthrough
 */
int valid_sla() {
	if (sla_loaded == 0)
		return 1;
	if (!json_is_object(curr_sla))
		return 2;
	json_t test_obj;
	test_obj = json_object_get(curr_sla, "version");
	if (!json_is_integer(test_obj)) {
		json_decref(test_obj);
		return 3;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "client_id");
	if (!json_is_string(test_obj)) {
		json_decref(test_obj);
		return 4;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "storage_id");
	if (!json_is_string(test_obj)) {
		json_decref(test_obj);
		return 5;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "storage_type");
	if (!json_is_string(test_obj)) {
		json_decref(test_obj);
		return 6;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "iops_max");
	if (!json_is_integer(test_obj)) {
		json_decref(test_obj);
		return 6;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "iops_min");
	if (!json_is_integer(test_obj)) {
		json_decref(test_obj);
		return 7;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "throughput_max");
	if (!json_is_integer(test_obj)) {
		json_decref(test_obj);
		return 8;
	}
	json_decref(test_obj);
	test_obj = json_object_get(curr_sla, "throughput_min");
	if (!json_is_integer(test_obj)) {
		json_decref(test_obj);
		return 9;
	}
	json_decref(test_obj);
	return 0;
}


