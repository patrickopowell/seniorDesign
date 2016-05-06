/**
 * QualiQueue - Spring 2016
 * @author Remington Campbell <racampbe@ncsu.edu
 *
 * Decodes and encodes protocol formats into usable structs.
 * Utilizes the Jansson C JSON parsing library.
 * For common usage between client and server.
 */
#include "qqjson.h"

/**
 * The format strings utilized by Jansson.
 * Specifies key-value pairs in a compact manner.
 */
// SLA format string
const char *sla_fmt = "{s:i, s:i, s:s, s:i, s:i, s:i, s:i, s:i, s:i, s:i}";
// Client Feedback format string
const char *cf_fmt = "{s:i, s:i, s:i, s:i, s:i, s:i, s:i, s:i, s:i, s:i}";

/**
 * Converts JSON object to string.
 */
char *qq_json_to_string(json_t *obj) {
	return json_dumps(obj, 0);
}

/**
 * Decode an SLA that is in string, JSON format into an SLA struct.
 * Return QQJSON_NOT_JSON if not in JSON format.
 * Return QQJSON_VALID if valid JSON.
 *
 * @author Remington Campbell <racampbe@ncsu.edu
 */
int qq_decode_sla(char *sla, struct sla *sla_dest)
{
	json_error_t error;
	json_t *curr_sla = json_loads(sla, 0, &error);
	if (!curr_sla) {
		qq_log_error("SLA not in JSON format!");
		return QQJSON_NOT_JSON;
	}
	json_unpack(curr_sla, sla_fmt, 
		"version", sla_dest->version,
		"sla_version", sla_dest->sla_version,
		"client_id", sla_dest->client_id,
		"storage_id", sla_dest->storage_id,
		"storage_type", sla_dest->storage_type,
		"iops_max", sla_dest->iops_max,
		"iops_min", sla_dest->iops_min,
		"throughput_min", sla_dest->throughput_min,
		"throughput_max", sla_dest->throughput_max,
		"unused_bandwidth", sla_dest->unused_bandwidth
	);
	return QQJSON_VALID;
}

/**
 * Encode an SLA in struct format into a JSON string.
 * Must free returned pointer after usage!
 *
 * @author Remington Campbell <racampbe@ncsu.edu
 */
char *qq_encode_sla(struct sla *sla)
{
	json_t *sla_json = json_pack(sla_fmt,
		"version", sla->version,
		"sla_version", sla->sla_version,
		"storage_id", sla->storage_id,
		"storage_type", sla->storage_type,
		"iops_max", sla->iops_max,
		"iops_min", sla->iops_min,
		"throughput_min", sla->throughput_min,
		"throughput_max", sla->throughput_max,
		"unused_bandwidth", sla->unused_bandwidth
	);
	char *json_str = qq_json_to_string(sla_json);
	json_decref(sla_json);
	return json_str;
}

/**
 * Decode client feedback that is in string, JSON format into a client feedback struct.
 * Return QQJSON_NOT_JSON if not in JSON format.
 * Return QQJSON_VALID if valid JSON.
 *
 * @author Remington Campbell <racampbe@ncsu.edu
 */
int qq_decode_cf(char *cf, struct client_feedback *cf_dest)
{
	json_error_t error;
	json_t *curr_cf = json_loads(cf, 0, &error);
	if (!curr_cf) {
		qq_log_error("Client feedback not in JSON format!");
		return QQJSON_NOT_JSON;
	}
	json_unpack(curr_cf, cf_fmt, 
		"version", cf_dest->version,
		"sla_version", cf_dest->sla_version,
		"storage_id", cf_dest->storage_id,
		"storage_type", cf_dest->storage_type,
		"current_throughput", cf_dest->current_throughput,
		"writes_queued", cf_dest->writes_queued,
		"reads_queued", cf_dest->reads_queued,
		"suspensions", cf_dest->suspensions,
		"sla_check", cf_dest->sla_check,
		"critical_request", cf_dest->critical_request
	);
	return QQJSON_VALID;
}

/**
 * Encode client feedback in struct format into a JSON string.
 * Must free returned pointer after usage!
 *
 * @author Remington Campbell <racampbe@ncsu.edu
 */
char *qq_encode_cf(struct client_feedback *cf)
{
	json_t *cf_json = json_pack(cf_fmt,
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
	if (json_str == NULL)
		qq_log_error("Could not encode client feedback!");
	json_decref(cf_json);
	return json_str;
}