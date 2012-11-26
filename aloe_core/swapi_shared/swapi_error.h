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

#ifndef SWAPI_ERROR_H
#define SWAPI_ERROR_H

#include <stddef.h>
#include <errno.h>
#include "str.h"
#include "defs.h"

#define SWAPI_ERROR_INVAL 	1
#define SWAPI_ERROR_HWAPI 	2
#define SWAPI_ERROR_OTHER 	3
#define SWAPI_ERROR_NOSPACE 	4
#define SWAPI_ERROR_LARGE 	5
#define SWAPI_ERROR_NOTFOUND 	6

/* Invalid parameter during a call to a swapi function */
#define SWAPI_ASSERT_PARAM(a) do { if(!(a)) { swapi_error_set(ctx,SWAPI_ERROR_INVAL, __FILE__, __LINE__); \
	return -1; } } while(0)

#define SWAPI_ASSERT_PARAM_P(a) do { if(!(a)) { swapi_error_set(ctx,SWAPI_ERROR_INVAL, __FILE__, __LINE__); \
	return NULL; } } while(0)

/* a hwapi call caused an error. Errno is provided by hwapi_error_code() */
#define SWAPI_HWERROR(msg)   swapi_error_hwapi(ctx,hwapi_error_code(), __FILE__, __LINE__, msg)

/* a swapi call caused an error. */
#define SWAPI_SETERROR(code)   swapi_error_set(ctx,code, __FILE__, __LINE__)

/* general error caused by a swapi call */
#define SWAPI_ERROR(msg)   swapi_error_set_msg(ctx,SWAPI_ERROR_OTHER, __FILE__, __LINE__, msg)

/* Called from outside API. Prints swapi error in one line and current file/line plus message in another */
#define swapi_perror(msg) swapi_error_print(ctx,""); aerror(msg)


typedef struct {
	int code;
	int sys_errnum;
	lstrdef(msg);
}swapi_error_t;

void swapi_error_hwapi(void *context, int errnum, const char *file, int line,
		const char *system_call);
void swapi_error_set(void *context, int code, const char *file, int line);
void swapi_error_set_msg(void *context, int code, const char *file, int line, const char *msg);
void swapi_error_print(void *context, const char *user_message);

#endif
