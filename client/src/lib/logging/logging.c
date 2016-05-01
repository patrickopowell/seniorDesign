#include "logging.h"

void qos_setup_logging(const char *id)
{
	openlog(id, LOG_CONS | LOG_PID | LOG_PERROR, LOG_DAEMON);
	qos_log_info("Starting qqclient logging.");
}

void qos_log_info(const char *message)
{
	syslog(LOG_INFO, message);
}

void qos_log_error(const char *message)
{
	syslog(LOG_ERR, message);
}

void qos_log_critical(const char *message)
{
	syslog(LOG_CRIT, message);
}

void qos_log_debug(const char *message)
{
	syslog(LOG_DEBUG, message);
}

void qos_halt_logging()
{
	qos_log_info("Halting qqclient logging.");
	closelog();
}