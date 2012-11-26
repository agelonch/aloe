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
#include "swapi_context.h"
#include "swapi_error.h"
#include "swapi.h"

inline static const char *print_error_string(void *ctx, int code);

void swapi_error_hwapi(void *ctx, int errnum, const char *file, int line,
		const char *system_call) {
	swapi_context_t *swapi_context = (swapi_context_t*) ctx;
	swapi_error_t *context = &swapi_context->error;
	assert(context);
	context->sys_errnum=errnum;
	context->code = SWAPI_ERROR_HWAPI;
	snprintf(context->msg, LSTR_LEN, "[file %s, line %d]: HWAPI call: %s. "
			"Error: %s", file, line, system_call, hwapi_error_string());
}

void swapi_error_set(void *context, int code, const char *file, int line) {
	swapi_error_set_msg(context, code,file,line,NULL);
}

void swapi_error_set_msg(void *ctx, int code, const char *file, int line, const char *msg) {
	swapi_context_t *swapi_context = (swapi_context_t*) ctx;
	swapi_error_t *context = &swapi_context->error;
	assert(context);
	context->code=code;
	snprintf(context->msg, LSTR_LEN, "[file %s, line %d]: %s", file, line,
			print_error_string(ctx,code));
	if (msg) {
		strncat(context->msg, msg, LSTR_LEN);
	}
}


void swapi_error_print(void *ctx, const char *user_message) {
	swapi_context_t *swapi_context = (swapi_context_t*) ctx;
	swapi_error_t *context = &swapi_context->error;
	assert(context);
	assert(user_message);
	if (strlen(user_message)) {
		fprintf(stderr, "%s: %s\n", context->msg, user_message);
	} else {
		fprintf(stderr, "%s\n", context->msg);
	}
}

inline static const char *print_error_string(void *ctx, int code) {
	swapi_context_t *swapi_context = (swapi_context_t*) ctx;
	swapi_error_t *context = &swapi_context->error;
	assert(context);
	switch(code) {
	case SWAPI_ERROR_INVAL:
		return "Invalid parameter. ";
	case SWAPI_ERROR_NOSPACE:
		return "Not enough space in structure array. ";
	case SWAPI_ERROR_LARGE:
		return "Too large. ";
	case SWAPI_ERROR_NOTFOUND:
		return "Object not found. ";
	default:
		return "";
	}

}

string swapi_error_string(void *ctx) {
	swapi_context_t *swapi_context = (swapi_context_t*) ctx;
	swapi_error_t *context = &swapi_context->error;
	assert(context);
	return context->msg;
}

int swapi_error_code(void *ctx) {
	swapi_context_t *swapi_context = (swapi_context_t*) ctx;
	swapi_error_t *context = &swapi_context->error;
	assert(context);
	return context->code;
}
