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

#ifndef HWAPI_ERROR_H
#define HWAPI_ERROR_H

#include <stddef.h>
#include <errno.h>
#include "str.h"
#include "defs.h"

#define HWAPI_ERROR_INVAL 	1
#define HWAPI_ERROR_SYSTEM 	2
#define HWAPI_ERROR_OTHER 	3
#define HWAPI_ERROR_NOSPACE 	4
#define HWAPI_ERROR_LARGE 	5
#define HWAPI_ERROR_NOTFOUND 	6
#define HWAPI_ERROR_DL		7

/* Invalid parameter during a call to a hwapi function */
#define HWAPI_ASSERT_PARAM(a) do { if(!(a)) { hwapi_error_set(HWAPI_ERROR_INVAL, __FILE__, __LINE__); \
	return -1; } } while(0)

#define HWAPI_ASSERT_PARAM_P(a) do { if(!(a)) { hwapi_error_set(HWAPI_ERROR_INVAL, __FILE__, __LINE__); \
	return NULL; } } while(0)

/* a posix system call caused an error. Errno is the value returned by the function */
#define HWAPI_POSERROR(a, msg)   hwapi_error_systemcall(a, __FILE__, __LINE__, msg)

/* a system call caused an error. Errno is provided by errno.h */
#define HWAPI_SYSERROR(msg)   hwapi_error_systemcall(errno, __FILE__, __LINE__, msg)

/* a dynamic library interface call caused an error.  */
#define HWAPI_DLERROR(msg)   hwapi_error_set_msg(HWAPI_ERROR_DL, __FILE__, __LINE__, msg)

/* a hwapi call caused an error. */
#define HWAPI_SETERROR(code)   hwapi_error_set(code, __FILE__, __LINE__)

/* general error caused by a hwapi call */
#define HWAPI_ERROR(msg)   hwapi_error_set_msg(HWAPI_ERROR_OTHER, __FILE__, __LINE__, msg)

/* Called from outside API. Prints hwapi error in one line and current file/line plus message in another */
#define hwapi_perror(msg) hwapi_error_print(""); aerror(msg)


typedef struct {
	int code;
	int sys_errnum;
	lstrdef(msg);
}hwapi_error_t;

void hwapi_error_set_context(hwapi_error_t *_context);
void hwapi_error_systemcall(int errnum, const char *file, int line, const char *system_call);
void hwapi_error_set(int code, const char *file, int line);
void hwapi_error_set_msg(int code, const char *file, int line, const char *msg);
void hwapi_error_print(const char *user_message);

#endif
