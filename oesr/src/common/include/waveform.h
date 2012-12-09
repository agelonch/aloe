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

#ifndef oesr_man_WAVEFORM_H_
#define oesr_man_WAVEFORM_H_

#include "rtdal.h"
#include "str.h"
#include "objects_max.h"

typedef struct {
	int module_ts;
	int node_ts;
	float mean_exec_us;
	int max_exec_us;
	float mean_rel_us;
	int max_rel_us;
	float mean_start_us;
	int max_start_us;
	float mean_mopts;
	float max_mopts;
	time_t t_exec[3];
	int last_update_ts;
	int start_ts;
} execinfo_t;

typedef struct {
	int id;
	int remote_port_idx;
	int remote_module_id;
	int physic_itf_id;
	float total_mbpts;
	float use_mbpts;
	int delay;
	h_itf_t hw_itf;
} interface_t;

typedef enum {
	VAR_TYPE_INT, VAR_TYPE_FLOAT, VAR_TYPE_STRING
}variable_type_t;

typedef struct {
	int id;
	int size;
	strdef(name);
	int window;
	int period;
	int periodCnt;
	void *init_value[MAX(modes)];
	void *cur_value;
	int nof_modes;
	variable_type_t type;
} variable_t;


typedef enum {
	PARSED, LOADED, INIT, RUN, STEP, PAUSE, STOP
} waveform_status_enum;

typedef struct {
	variable_t *variables;
	interface_t *inputs;
	interface_t *outputs;
	execinfo_t execinfo;
	void *waveform;
	void *node;
	int id;
	int last_update_ts;
	strdef(name);
	strdef(binary);
	float c_mopts[MAX(modes)];
	int processor_idx;
	int exec_position;
	waveform_status_enum status;
	int nof_reporting_vars;
	int nof_variables;
	int nof_inputs;
	int nof_outputs;
	int cur_mode;
	int nof_modes;
} module_t;


typedef struct {
	waveform_status_enum cur_status;
	int next_timeslot;
} waveform_status_t;

typedef struct {
	strdef(name);
	strdef(desc);
	strdef(opts);
}waveform_mode_t;

/**
 * Inherits from General.Waveform class. After the waveform is loaded to a platform,
 the class allows to send status change commands and update the local objects with the
 remote ALOE-Node waveform objects. A waveform must be parsed (parse()) prior to being
 loaded (load()).
 */

typedef struct {
	int id;
	module_t *modules;
	waveform_status_t status;
	waveform_mode_t modes[MAX(modes)];
	int last_update_ts;
	int status_timestamp_delay;
	int modules_x_node[MAX(nodes)];
	float *c;
	float **b;
	int nof_modules;
	strdef(model_file);
	strdef(name);
	int nof_modes;
} waveform_t;

module_t* waveform_find_module_id(waveform_t *w, int obj_id);
module_t* waveform_find_module_name(waveform_t *w, char *name);
int module_alloc(module_t *module, int nof_inputs, int nof_outputs, int nof_variables);
int module_free(module_t *module);
int waveform_alloc(waveform_t *waveform, int nof_modules);
int waveform_free(waveform_t *waveform);
int variable_alloc(variable_t *variable, int nof_modes);
int variable_free(variable_t *variable);
#endif
