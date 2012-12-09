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

#ifndef rtdal_ERROR_H
#define rtdal_ERROR_H

#include <stddef.h>
#include <errno.h>
#include "str.h"
#include "defs.h"

#define rtdal_ERROR_INVAL 	1
#define rtdal_ERROR_SYSTEM 	2
#define rtdal_ERROR_OTHER 	3
#define rtdal_ERROR_NOSPACE 	4
#define rtdal_ERROR_LARGE 	5
#define rtdal_ERROR_NOTFOUND 	6
#define rtdal_ERROR_DL		7

/* Invalid parameter during a call to a rtdal function */
#define rtdal_ASSERT_PARAM(a) do { if(!(a)) { rtdal_error_set(rtdal_ERROR_INVAL, __FILE__, __LINE__); \
	return -1; } } while(0)

#define rtdal_ASSERT_PARAM_P(a) do { if(!(a)) { rtdal_error_set(rtdal_ERROR_INVAL, __FILE__, __LINE__); \
	return NULL; } } while(0)

/* a posix system call caused an error. Errno is the value returned by the function */
#define rtdal_POSERROR(a, msg)   rtdal_error_systemcall(a, __FILE__, __LINE__, msg)

/* a system call caused an error. Errno is provided by errno.h */
#define rtdal_SYSERROR(msg)   rtdal_error_systemcall(errno, __FILE__, __LINE__, msg)

/* a dynamic library interface call caused an error.  */
#define rtdal_DLERROR(msg)   rtdal_error_set_msg(rtdal_ERROR_DL, __FILE__, __LINE__, msg)

/* a rtdal call caused an error. */
#define rtdal_SETERROR(code)   rtdal_error_set(code, __FILE__, __LINE__)

/* general error caused by a rtdal call */
#define rtdal_ERROR(msg)   rtdal_error_set_msg(rtdal_ERROR_OTHER, __FILE__, __LINE__, msg)


typedef struct {
	int code;
	int sys_errnum;
	lstrdef(msg);
}rtdal_error_t;

void rtdal_error_set_context(rtdal_error_t *_context);
void rtdal_error_systemcall(int errnum, const char *file, int line, const char *system_call);
void rtdal_error_set(int code, const char *file, int line);
void rtdal_error_set_msg(int code, const char *file, int line, const char *msg);
int rtdal_error_code();

#endif
