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
#include "str.h"
#include "swapi_context.h"
#include "swapi_error.h"
#include "swapi.h"

static strdef(error_buffer);

void swapi_error_hwapi(void *ctx, const char *msg) {
	swapi_error_set(ctx, 0, msg);
}

void swapi_error_set(void *ctx, swapi_errorcodes_t code, const char *msg) {
	CAST(swapi,ctx,swapi_context_t*);
	swapi->error.code=code;
	strcpy(swapi->error.msg, msg);
}

void swapi_error_print(void *ctx) {
	fprintf(stderr, "[file %s, line %d]: %s",__FILE__, __LINE__, swapi_error_string(ctx));
}

string swapi_error_string(void *ctx) {
	CAST(swapi,ctx,swapi_context_t*);
	switch(swapi->error.code) {
	case 0:
		sprintf(error_buffer, "[hwapi error: %s]",hwapi_error_string());
		break;
	default:
		sprintf(error_buffer, "uknown error: ");
		break;
	}
	sprintf(error_buffer, "%s %s\n", error_buffer, swapi->error.msg);
	return error_buffer;
}

int swapi_error_code(void *ctx) {
	CAST(swapi,ctx,swapi_context_t*);
	return (int) swapi->error.code;
}
