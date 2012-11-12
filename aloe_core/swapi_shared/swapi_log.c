#include "swapi_log.h"
#include "swapi.h"
#include "defs.h"

int swapi_log_init(log_t log, string name) {
	aerror("Not yet implemented");
	return -1;
}

int swapi_log_close(log_t log) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Concatenates the string to buffer
 */
int swapi_log_write(log_t log, string str) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Use aerror to format string and concatenates to buffer
 */
int swapi_log_laerror(log_t log, int va_args) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Writes the contents of the buffer. Called by runCycle at the end of the timeslot.
 */
static int __attribute__((__unused__)) _write_log(log_t log) {
	aerror("Not yet implemented");
	return -1;
}
