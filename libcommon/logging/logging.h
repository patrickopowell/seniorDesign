#include <syslog.h>

void qq_setup_logging(const char *id);
void qq_log_info(const char *message);
void qq_log_error(const char *message);
void qq_log_critical(const char *message);
void qq_log_debug(const char *message);
void qq_halt_logging();