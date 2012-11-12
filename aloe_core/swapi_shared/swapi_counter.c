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
