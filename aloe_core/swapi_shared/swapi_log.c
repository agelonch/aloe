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
#include "str.h"
#include "swapi_context.h"
#include "nod_waveform.h"

int swapi_log_init(void *context, swapi_log_t *log, string name) {
	swapi_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	lstrdef(tmp);
	SWAPI_ASSERT_PARAM(log);
	SWAPI_ASSERT_PARAM(name);
	snprintf(tmp,LSTR_LEN,"%s.%s",module->parent.name,name);
	int fd = hwapi_file_open(tmp);
	if (fd < 0) {
		return -1;
	}
	log->fd = fd;
	log->context = ctx;
	strcpy(log->name,name);
	return 0;
}

int swapi_log_close(log_t log) {
	swapi_log_t *l = (swapi_log_t*) log;
	swapi_context_t *ctx = l->context;
	SWAPI_ASSERT_PARAM(log);
	if (hwapi_file_close(l->fd)) {
		return -1;
	}
	l->fd = 0;
	l->id = 0;
	return 0;
}

/**
 * Concatenates the string to buffer
 */
int swapi_log_write(log_t log, string str) {
	swapi_log_t *l = (swapi_log_t*) log;
	swapi_context_t *ctx = l->context;
	SWAPI_ASSERT_PARAM(log);
	strncat(l->buffer,str,LSTR_LEN);
	return 0;
}

/**
 * Writes the contents of the buffer. Called by runCycle at the end of the timeslot.
 */
static int __attribute__((__unused__)) _write_log(log_t log) {
	aerror("Not yet implemented");
	return -1;
}
