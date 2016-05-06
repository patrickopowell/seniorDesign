#include "logging.h"

void qq_setup_logging(const char *id)
{
	openlog(id, LOG_CONS | LOG_PID | LOG_PERROR, LOG_DAEMON);
	qq_log_info("Starting qqclient logging.");
}

void qq_log_info(const char *message)
{
	syslog(LOG_INFO, message);
}

void qq_log_error(const char *message)
{
	syslog(LOG_ERR, message);
}

void qq_log_critical(const char *message)
{
	syslog(LOG_CRIT, message);
}

void qq_log_debug(const char *message)
{
	syslog(LOG_DEBUG, message);
}

void qq_halt_logging()
{
	qq_log_info("Halting qqclient logging.");
	closelog();
}