
#include "swapi_counter.h"
#include "swapi.h"

/**
 * 1) save name
 * 2) find a free ModuleVariable in mymodule and fill attributes
 * 3) call hwapi.openShm()
 * 4) save pointer to shmPtr
 */
int swapi_counter_init(counter_t counter, string name) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Releases the variable's shared memory and frees the ModuleVariable object
 */
int swapi_counter_close(counter_t counter) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Starts counting time
 */
int swapi_counter_start(counter_t counter) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Stops counting time and saves the elapsed microseconds in the variable.
 */
int swapi_counter_stop(counter_t counter) {
	aerror("Not yet implemented");
	return -1;
}
