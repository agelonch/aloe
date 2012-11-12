#ifndef SWAPI_LOG_H
#define SWAPI_LOG_H

#include "str.h"
#include "swapi.h"

/**
 * A Log is a mechanism for the module to write text to a logfile without interrupting real-time processing. Successive calls to Log() or Write() functions store the string in a temporal buffer. At the end of the timeslot, a new thread with lower priority is created to write the contents a non-volatile media provided by the hwapi (e.g. a hard-drive or flash-rom). Note that it may happen that the contents are never written, in the case that the processor is 100% of its time running signal processing tasks.
 */
typedef struct {
	int id;
	strdef(name);
	strdef(buffer);
	int fd;
	int w_ptr;
}swapi_log_t;

int swapi_log_init(log_t log, string name);

#endif
