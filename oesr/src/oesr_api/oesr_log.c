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

#include <string.h>
#include <stdarg.h>

#include "oesr_log.h"
#include "oesr.h"
#include "defs.h"
#include "str.h"
#include "oesr_context.h"
#include "nod_waveform.h"

lstrdef(tmp_buffer);

/**
 * Called from oesr_log_create. Initializes the structure and creates the associated
 * filename for writing logs.
 */
int oesr_log_init(void *context, oesr_log_t *log, string name) {
	oesr_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	sdebug("log_name=%s\n",name);
	lstrdef(tmp);
	OESR_ASSERT_PARAM(log);
	OESR_ASSERT_PARAM(name);
	snprintf(tmp,LSTR_LEN,"%s.%s",module->parent.name,name);
	sdebug("filename=%s\n",tmp);
	int fd = rtdal_file_open(tmp);
	if (fd < 0) {
		OESR_HWERROR("rtdal_file_open");
		return -1;
	}
	sdebug("log_fd=%d\n",fd);
	log->fd = fd;
	log->context = ctx;
	strcpy(log->name,name);
	return 0;
}

/**
 * Closes a log associated to the handler passed as parameter.
 * \param log Handler obtained by oesr_log_create()
 *
 * \returns 0 on success, -1 on error
 */
int oesr_log_close(log_t log) {
	oesr_log_t *l = (oesr_log_t*) log;
	oesr_context_t *ctx = l->context;
	sdebug("log_id=%s, fd=%d\n",l->id, l->fd);
	OESR_ASSERT_PARAM(log);
	if (rtdal_file_close(l->fd)) {
		OESR_HWERROR("rtdal_file_close");
		return -1;
	}
	l->fd = 0;
	l->id = 0;
	return 0;
}

/**
 * Writes the buffer str to the log file associated to the handler log.
 * Only up to LSTR_LEN characters of the string will be copied to the log buffer.
 *
 * \param log Handler obtained by oesr_log_create()
 * \param str String to write to the buffer
 *
 * \returns 0 on success, -1 on error
 */
int oesr_log_write(log_t log, char *str) {
	oesr_log_t *l = (oesr_log_t*) log;
	oesr_context_t *ctx = l->context;
	OESR_ASSERT_PARAM(log);
	OESR_ASSERT_PARAM(str);
	sdebug("log_id=%s, buffer_len=%d\n",l->id, strnlen(l->buffer,LSTR_LEN));
	strncat(l->buffer,str,LSTR_LEN);
	return 0;
}

/**
 * Writes a formated string to the log file associated to the handler log.
 * \param log Handler obtained by oesr_log_create()
 * \param fmt String format
 *
 * \returns 0 on success, -1 on error
 */
int oesr_log_printf(log_t log, const char *fmt, ...) {
	oesr_log_t *l = (oesr_log_t*) log;
	oesr_context_t *ctx = l->context;
	OESR_ASSERT_PARAM(log);
	OESR_ASSERT_PARAM(fmt);
	va_list args;
	va_start(args,fmt);
	vsnprintf(tmp_buffer,LSTR_LEN,fmt,args);
	return oesr_log_write(log,tmp_buffer);
}

/**
 * Writes the contents of the buffer. Called by runCycle at the end of the timeslot.
 */
static int __attribute__((__unused__)) _write_log(log_t log) {
	aerror("Not yet implemented");
	return -1;
}
