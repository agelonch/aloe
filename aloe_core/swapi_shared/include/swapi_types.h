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

typedef enum {
	READ, WRITE
}swapi_itf_mode_t;

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
