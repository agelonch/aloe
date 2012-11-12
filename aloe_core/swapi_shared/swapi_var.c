#include <stddef.h>
#include "swapi_var.h"
#include "swapi.h"

/**
 * Declares a module variable as "public". Then other modules or the manager can view and modify it.
 *
 * 1) Find if variable name exists in mymodule.variables, otherwise find an empty space and fill attributes
 * 2) call hwapi.openShm() to create a shared memory area for the variable. Save pointer in the variable object found in previous step
 * 3) If all ok, return the shared memory pointer, otherwise return null
 */
int swapi_var_init(var_t var, string name, int size, void **ptr) {
	aerror("Not yet implemented");
	return 0;
}

/**
 * Sets the initialization value of a variable to the buffer pointed by ptr
 *
 * 1) find a variable in mymodule.variables with mymodule.variables[i].name=name
 * 2) copy min(size,mymodule.variables[i].size) bytes of mymodule.variables[i].initValue to ptr
 */
int swapi_var_initialize(string name, void* ptr, int size) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Declares a public variable "private" so that any other module or the manager can modify it
 *
 * 1) Find a variable in mymodule.variables with mymodule.variables[i].shmPtr=ptr
 * 2) clear mymodule.variables[i] attributes
 * 3) call hwapi.closeShm(ptr);
 */
int swapi_var_close(var_t var) {
	aerror("Not yet implemented");
	return -1;
}
