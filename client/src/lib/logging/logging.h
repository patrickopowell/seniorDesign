#include <syslog.h>

void qos_setup_logging(const char *id);
void qos_log_info(const char *message);
void qos_log_error(const char *message);
void qos_log_critical(const char *message);
void qos_log_debug(const char *message);
void qos_halt_logging();