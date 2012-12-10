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

#ifndef oesr_man_ERROR_H
#define oesr_man_ERROR_H

#include "str.h"

typedef enum {
	INVAL,NOSPACE, EXISTS, rtdal, BIG, OTHER
} oesr_man_errorcodes_t;

typedef struct {
	oesr_man_errorcodes_t code;
	strdef(msg);
}oesr_man_error_t;

void oesr_man_error_set_context(oesr_man_error_t *context);
void oesr_man_error_rtdal(const char *msg);
void oesr_man_error_set(oesr_man_errorcodes_t code, const char *msg);

#endif
