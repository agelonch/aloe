/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

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
