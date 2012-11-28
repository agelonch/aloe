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

#ifndef SWAPI_H_
#define SWAPI_H_

#include "swapi_types.h"

#define SWAPI_ITF_DEFAULT_MSG (64*1024)

int swapi_exit(void *context);
int swapi_tstamp(void *context);

char *swapi_module_name(void *context);
int swapi_module_id(void *context);

itf_t swapi_itf_create(void *context, int port_idx, swapi_itf_mode_t mode, int size);
int swapi_itf_close(itf_t itf);
int swapi_itf_write(itf_t itf, void* buffer, int size);
int swapi_itf_read(itf_t itf, void* buffer, int size);
int swapi_itf_status(itf_t itf);
void *swapi_itf_ptr_request(itf_t itf);
int swapi_itf_ptr_release(itf_t itf, void *ptr);
int swapi_itf_ptr_put(itf_t itf, void *ptr, int len);
void *swapi_itf_ptr_get(itf_t itf, int *len);

var_t swapi_var_create(void *context, char* name, int size, void **ptr);
int swapi_var_initialize(void *context, char* name, void* ptr, int size);
int swapi_var_close(void *context, var_t var);

counter_t swapi_counter_create(void *context, char* name);
int swapi_counter_close(counter_t counter);
int swapi_counter_start(counter_t counter);
int swapi_counter_stop(counter_t counter);


log_t swapi_log_create(void *context, char* name);
int swapi_log_close(log_t log);
int swapi_log_write(log_t log, char *str);

#endif
