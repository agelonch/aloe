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

#include "oesr_man_error.h"
#include "oesr_man.h"

static oesr_man_error_t *context;

void oesr_man_error_set_context(oesr_man_error_t *_context) {
	context = _context;
}

static strdef(error_buffer);

void oesr_man_error_rtdal(const char *msg) {
	oesr_man_error_set(0, msg);
}

void oesr_man_error_set(oesr_man_errorcodes_t code, const char *msg) {
	context->code=code;
	strcpy(context->msg, msg);
}

void oesr_man_error_print(void *ctx) {
	fprintf(stderr, "[file %s, line %d]: %s",__FILE__, __LINE__, oesr_man_error_string(ctx));
}

string oesr_man_error_string(void *ctx) {
	switch(context->code) {
	case 0:
		sprintf(error_buffer, "[rtdal error: %s]",rtdal_error_string());
		break;
	default:
		sprintf(error_buffer, "uknown error: ");
		break;
	}
	sprintf(error_buffer, "%s %s\n", error_buffer, context->msg);
	return error_buffer;
}

int oesr_man_error_code() {
	return (int) context->code;
}
