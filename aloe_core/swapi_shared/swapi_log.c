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
