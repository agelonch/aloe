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
#include "hwapi_context.h"
#include "hwapi_error.h"
#include "hwapi.h"

static hwapi_error_t *context = NULL;

inline static const char *print_error_string(int code);

void hwapi_error_set_context(hwapi_error_t *_context) {
	assert(_context);
	context = _context;
}

void hwapi_error_systemcall(int errnum, const char *file, int line,
		const char *system_call) {
	assert(context);
	context->sys_errnum=errnum;
	context->code = HWAPI_ERROR_SYSTEM;
	snprintf(context->msg, LSTR_LEN, "[file %s, line %d]: System call: %s. "
			"Error: %s", file, line, system_call, strerror(errnum));
}

void hwapi_error_set(int code, const char *file, int line) {
	hwapi_error_set_msg(code,file,line,NULL);
}

void hwapi_error_set_msg(int code, const char *file, int line, const char *msg) {
	assert(context);
	context->code=code;
	snprintf(context->msg, LSTR_LEN, "[file %s, line %d]: %s", file, line,
			print_error_string(code));
	if (msg) {
		strncat(context->msg, msg, LSTR_LEN);
	}
}


void hwapi_error_print(const char *user_message) {
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
	case HWAPI_ERROR_INVAL:
		return "Invalid parameter. ";
	case HWAPI_ERROR_NOSPACE:
		return "Not enough space in structure array. ";
	case HWAPI_ERROR_LARGE:
		return "Too large. ";
	case HWAPI_ERROR_DL:
		return "Dynamic Loading. ";
	case HWAPI_ERROR_NOTFOUND:
		return "Object not found. ";
	default:
		return "";
	}

}

string hwapi_error_string() {
	assert(context);
	return context->msg;
}

int hwapi_error_code() {
	assert(context);
	return context->code;
}
