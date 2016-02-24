#include "logging.h"

int log_fd;
zlog_category_t *info_log;
zlog_category_t *error_log;

int qos_setup_logging() {
	log_fd = dzlog_init(ZLOG_CONF_LOCATION, "qos");
	if (log_fd) {
		printf("Logging initialization failed.\n");
		return -1;
	}
	qos_log_info("Logging initialized.");
	return 0;
}

void qos_log_info(char *message) {
	zlog_info(info_log, message);
}

void qos_log_error(char *message) {
	zlog_info(error_log, message);
}

void qos_halt_logging() {
	qos_log_info("Request to halt logging received.");
	zlog_fini();
}