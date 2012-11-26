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

#ifndef HWAPI_TYPES_H_
#define HWAPI_TYPES_H_

#include <time.h>
#define time_t struct timeval

#include "str.h"
#include "hwapi_types.h"

struct h_proc_ {
	int id;
};
typedef struct h_proc_* h_proc_t;

/**
 * Process attributes for use with the hwapi_process_new() function.
 */
struct hwapi_process_attr {
	strdef(binary_path);
	int pipeline_id;
	int exec_position;
	void (*finish_callback)(h_proc_t);
};


typedef struct {
	int len;
	void *data;
}pkt_t;

struct h_itf_ {
	int id;
	int is_external;
};
typedef struct h_itf_* h_itf_t;


struct h_dac_ {
	int id;
};
typedef struct h_dac_* h_dac_t;


#endif
