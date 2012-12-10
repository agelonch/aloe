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
#include <stdio.h>
#include <errno.h>
#include "str.h"
#include "rtdal_context.h"
#include "rtdal_error.h"
#include "rtdal.h"

static rtdal_error_t *context = NULL;

inline static const char *print_error_string(int code);

void rtdal_error_set_context(rtdal_error_t *_context) {
	assert(_context);
	context = _context;
}

void rtdal_error_systemcall(int errnum, const char *file, int line,
		const char *system_call) {
	assert(context);
	context->sys_errnum=errnum;
	context->code = RTDAL_ERROR_SYSTEM;
	snprintf(context->msg, LSTR_LEN, "[file %s, line %d]: System call: %s. "
			"Error: %s", file, line, system_call, strerror(errnum));
}

void rtdal_error_set(int code, const char *file, int line) {
	rtdal_error_set_msg(code,file,line,NULL);
}

void rtdal_error_set_msg(int code, const char *file, int line, const char *msg) {
	assert(context);
	context->code=code;
	snprintf(context->msg, LSTR_LEN, "[file %s, line %d]: %s", file, line,
			print_error_string(code));
	if (msg) {
		strncat(context->msg, msg, LSTR_LEN);
	}
}

/**
 * Prints to stderr the current error message accompanied by a user message.
 */
void rtdal_error_print(const char *user_message) {
	assert(context);
	assert(user_message);
	if (strlen(user_message)) {
		fprintf(stderr, "%s: %s\n", context->msg, user_message);
	} else {
		fprintf(stderr, "%s\n", context->msg);
	}
}

inline static const char *print_error_string(int code) {
	assert(context);
	switch(code) {
	case RTDAL_ERROR_INVAL:
		return "Invalid parameter. ";
	case RTDAL_ERROR_NOSPACE:
		return "Run out of space. ";
	case RTDAL_ERROR_LARGE:
		return "Too large. ";
	case RTDAL_ERROR_DL:
		return "Dynamic Loading. ";
	case RTDAL_ERROR_NOTFOUND:
		return "Object not found. ";
	default:
		return "";
	}

}

/**
 * Returns a pointer tot he current error message
 */
string rtdal_error_string() {
	assert(context);
	return context->msg;
}

/**
 * Returns the current error code
 */
int rtdal_error_code() {
	assert(context);
	return context->code;
}
