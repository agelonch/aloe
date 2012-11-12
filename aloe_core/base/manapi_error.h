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

#ifndef MANAPI_ERROR_H
#define MANAPI_ERROR_H

#include "str.h"

typedef enum {
	INVAL,NOSPACE, EXISTS, HWAPI, BIG, OTHER
} manapi_errorcodes_t;

typedef struct {
	manapi_errorcodes_t code;
	strdef(msg);
}manapi_error_t;

void manapi_error_set_context(manapi_error_t *context);
void manapi_error_hwapi(const char *msg);
void manapi_error_set(manapi_errorcodes_t code, const char *msg);

#endif
