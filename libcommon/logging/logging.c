/**
 * QualiQueue - Spring 2016
 * @author Remington Campbell <racampbe@ncsu.edu
 *
 * Abstraction of logging methods for usage in QualiQueue system.
 * Currently utilizes syslog.
 */
#include "logging.h"

/**
 * Setup logging capabilities.
 */
void qq_setup_logging(const char *id)
{
	openlog(id, LOG_CONS | LOG_PID | LOG_PERROR, LOG_DAEMON);
	qq_log_info("Starting qqclient logging.");
}

/** Log informational message. **/
void qq_log_info(const char *message)
{
	syslog(LOG_INFO, message);
}

/** Log an error message. **/
void qq_log_error(const char *message)
{
	syslog(LOG_ERR, message);
}

/** Log message indicating critical impact. **/
void qq_log_critical(const char *message)
{
	syslog(LOG_CRIT, message);
}

/** Log message with debug information. **/
void qq_log_debug(const char *message)
{
	syslog(LOG_DEBUG, message);
}

/** Halt current logging capabilities. **/
void qq_halt_logging()
{
	qq_log_info("Halting qqclient logging.");
	closelog();
}