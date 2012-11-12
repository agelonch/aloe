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

#include "str.h"

typedef enum {
	INVAL, NOSPACE, EXISTS, HWAPI,BIG, OTHER
}swapi_errorcodes_t;

typedef struct {
	int id;
	swapi_errorcodes_t code;
	strdef(msg);
}swapi_error_t;

void swapi_error_hwapi(void *ctx, const char *msg);
void swapi_error_set(void *ctx, swapi_errorcodes_t code, const char *msg);

#endif
