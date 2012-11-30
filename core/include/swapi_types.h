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


#ifndef SWAPI_TYPES_H_
#define SWAPI_TYPES_H_

#define SWAPI_ERROR_INVAL 	1
#define SWAPI_ERROR_HWAPI 	2
#define SWAPI_ERROR_OTHER 	3
#define SWAPI_ERROR_NOSPACE 	4
#define SWAPI_ERROR_LARGE 	5
#define SWAPI_ERROR_NOTFOUND 	6
#define SWAPI_ERROR_NOTREADY 	6

typedef enum {
	ITF_READ, ITF_WRITE
}swapi_itf_mode_t;

/** must be exactly the same than h_pkt_t in hwapi_types.h */
typedef struct {
	int len;
	void *data;
}pkt_t;

struct _s_itf {
	int id;
};
typedef struct _s_itf* itf_t;

struct _s_var {
	int id;
};
typedef struct _s_var* var_t;

struct _s_counter {
	int id;
};
typedef struct _s_counter* counter_t;

struct _s_log {
	int id;
};
typedef struct _s_log* log_t;


#endif /* SWAPI_TYPES_H_ */
