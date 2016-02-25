#include "zlog.h"

#define ZLOG_CONF_LOCATION "/etc/zlog.conf"

int qos_setup_logging();
void qos_log_info(char *);
void qos_log_error(char *);
void qos_halt_logging();