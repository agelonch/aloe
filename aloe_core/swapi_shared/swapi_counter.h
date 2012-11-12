#ifndef SWAPI_COUNTER_H
#define SWAPI_COUNTER_H

#include "str.h"
#include "nod_variable.h"
#include "defs.h"
#include "swapi.h"

/**
 * A Counter is a class used to count short periods of time in a timeslot execution cycle, for instance, to measure the execution time of some parts of the code. It is associated with a public variable the name passed to the create() function. After a call to Stop(), the number of microseconds elapsed since the previous call to Start() is saved in the variable.
 */
typedef struct {
	int id;
	time_t count[3];
	nod_variable_t variable;
}swapi_counter_t;

int swapi_counter_init(counter_t counter, string name);

#endif
