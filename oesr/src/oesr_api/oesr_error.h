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

#ifndef oesr_ERROR_H
#define oesr_ERROR_H

#include <stddef.h>
#include <errno.h>
#include "str.h"
#include "defs.h"


/* Invalid parameter during a call to a oesr function */
#define oesr_ASSERT_PARAM(a) do { if(!(a)) { oesr_error_set(ctx,oesr_ERROR_INVAL, __FILE__, __LINE__); \
	return -1; } } while(0)

#define oesr_ASSERT_PARAM_P(a) do { if(!(a)) { oesr_error_set(ctx,oesr_ERROR_INVAL, __FILE__, __LINE__); \
	return NULL; } } while(0)

/* a rtdal call caused an error. Errno is provided by rtdal_error_code() */
#define oesr_HWERROR(msg)   oesr_error_rtdal(ctx,rtdal_error_code(), __FILE__, __LINE__, msg)

/* a oesr call caused an error. */
#define oesr_SETERROR(code)   oesr_error_set(ctx,code, __FILE__, __LINE__)

/* general error caused by a oesr call */
#define oesr_ERROR(msg)   oesr_error_set_msg(ctx,oesr_ERROR_OTHER, __FILE__, __LINE__, msg)


typedef struct {
	int code;
	int sys_errnum;
	lstrdef(msg);
}oesr_error_t;

void oesr_error_rtdal(void *context, int errnum, const char *file, int line,
		const char *system_call);
void oesr_error_set(void *context, int code, const char *file, int line);
void oesr_error_set_msg(void *context, int code, const char *file, int line, const char *msg);



#endif
