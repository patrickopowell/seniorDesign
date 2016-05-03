#include "qqjson.h"

const char *sla_fmt = "{s:i, s:i, s:s, s:i, s:i, s:i, s:i, s:i, s:i, s:i}";
const char *cf_fmt = "{s:i, s:i, s:s, s:i, s:i, s:i, s:i, s:i, s:i, s:i, s:i}";

/**
 * Converts JSON object to string for communication.
 */
char *qq_json_to_string(json_t *obj) {
	return json_dumps(obj, 0);
}

/** SLA PARSING **/

int qq_decode_sla(char *sla, struct sla *sla_dest)
{
	json_error_t error;
	json_t *curr_sla = json_loads(sla, 0, &error);
	if (!curr_sla) {
		qq_log_error("SLA not in JSON format!");
		return QQJSON_NOT_JSON;
	}
	json_unpack(curr_sla, sla_fmt, 
		"version", &(sla_dest->version),
		"sla_version", &(sla_dest->sla_version),
		"client_id", &(sla_dest->client_id),
		"storage_id", &(sla_dest->storage_id),
		"storage_type", &(sla_dest->storage_type),
		"iops_max", &(sla_dest->iops_max),
		"iops_min", &(sla_dest->iops_min),
		"throughput_min", &(sla_dest->throughput_min),
		"throughput_max", &(sla_dest->throughput_max),
		"unused_bandwidth", &(sla_dest->unused_bandwidth)
	);
	return QQJSON_VALID;
}

char *qq_encode_sla(struct sla *sla)
{
	json_t *sla_json = json_pack(sla_fmt,
		"version", cf->version,
		"sla_version", cf->sla_version,
		"storage_id", cf->storage_id,
		"storage_type", cf->storage_type,
		"iops_max", cf->iops_max,
		"iops_min", cf->iops_min,
		"throughput_min", cf->throughput_min,
		"throughput_max", cf->throughput_max,
		"unused_bandwidth", cf->unused_bandwidth,
	);
	char *json_str = qq_json_to_string(sla_json);
	json_decref(sla_json);
	return json_str;
}

int qq_decode_cf(char *cf, struct client_feedback *cf_dest)
{
	json_error_t error;
	json_t *curr_cf = json_loads(cf, 0, &error);
	if (!curr_cf) {
		qq_log_error("Client feedback not in JSON format!");
		return QQJSON_NOT_JSON;
	}
	json_unpack(curr_cf, cf_fmt, 
		"version", &(cf_dest->version),
		"sla_version", &(cf_dest->sla_version),
		"storage_id", &(cf_dest->storage_id),
		"storage_type", &(cf_dest->storage_type),
		"current_throughput", &(cf_dest->current_throughput),
		"writes_queued", &(cf_dest->writes_queued),
		"reads_queued", &(cf_dest->reads_queued),
		"suspensions", &(cf_dest->suspensions),
		"sla_check", &(cf_dest->sla_check),
		"critical_request", &(cf_dest->critical_request)
	);
	return QQJSON_VALID;
}

int qq_encode_cf(struct client_feedback *cf)
{
	json_t *cf_json = json_pack(feedback_fmt,
		"version", cf->version,
		"sla_version", cf->sla_version,
		"storage_id", cf->storage_id,
		"storage_type", cf->storage_type,
		"current_throughput", cf->current_throughput,
		"writes_queued", cf->writes_queued,
		"reads_queued", cf->reads_queued,
		"suspensions", cf->suspensions,
		"sla_check", cf->sla_check,
		"critical_request", cf->critical_request
	);
	char *json_str = qq_json_to_string(cf_json);
	json_decref(cf_json);
	return json_str;
}