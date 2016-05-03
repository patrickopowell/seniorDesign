#ifndef JANSSON_INCLUDE
#include <jansson.h>
#include "../protocol.h"
#include "../logging/logging.h"

#define QQJSON_VALID 0
#define QQJSON_NOT_JSON -1
#define QQJSON_NOT_VALID -2

char *qqjson_to_string(json_t *obj);
int qq_decode_sla(char *sla, struct sla *sla_dest);
char *qq_encode_sla(struct sla *sla);
int qq_decode_cf(char *cf, struct client_feedback *cf_dest);
char *qq_encode_cf(struct client_feedback *cf);

#define JANSSON_INCLUDE
#endif