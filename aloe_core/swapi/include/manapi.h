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

#ifndef MANAPI_H_
#define MANAPI_H_

#include "str.h"
#include "manapi_types.h"
#include "waveform.h"

int manapi_initialize(string platform_conf, int nof_waveforms);
int manapi_read_waveforms(string repository);
int manapi_start_nodes();
int manapi_stop_nodes();
waveform_t* manapi_waveform_get(string name);

int manapi_error_code();
char* manapi_error_string();
void manapi_error_print();

int update(updateable *obj);

int man_platform_update();
int man_platform_connect();
int man_platform_disconnect();
int man_platform_tslot_length();
man_platform_model_t* man_platform_get_model();

int variable_report_start(variable_t *var, void (*callback)(void), int period, int window);
int variable_report_stop(variable_t *var);

int waveform_load(waveform_t* waveform);
int waveform_parse(waveform_t* waveform);

int waveform_status_set(waveform_t *waveform, waveform_status_t *new_status);
int waveform_status_is_loaded(waveform_status_t* status);
int waveform_status_is_running(waveform_status_t* status);


#endif
