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

#ifndef NOD_WAVEFORM_H
#define NOD_WAVEFORM_H

#include "objects_max.h"
#include "str.h"
#include "serializable.h"
#include "waveform.h"
#include "packet.h"
#include "hwapi_types.h"

#define LOAD_TSLOT_DEAD 2
#define STOP_TSLOT_DEAD 5

typedef struct {
	module_t parent;
	h_proc_t process;
	void *context;
	int changing_status;
	int (*init) (void*);
	int (*stop) (void*);
} nod_module_t;

typedef struct {
	nod_module_t *modules;
	waveform_status_t status;
	waveform_mode_t modes[MAX(modes)];
	int nof_modes;
	int nof_modules;
	int id;
	int finishing;
	strdef(name);
} nod_waveform_t;


int nod_waveform_alloc(nod_waveform_t *w, int nof_modules);
int nod_waveform_load(nod_waveform_t *waveform);
int nod_waveform_run(nod_waveform_t *waveform, int runnable);
int nod_waveform_remove(nod_waveform_t *waveform);
int nod_waveform_status_new(nod_waveform_t *waveform, waveform_status_t *new_status);
int nod_waveform_status_stop(nod_waveform_t *waveform);
nod_module_t* nod_waveform_find_module_id(nod_waveform_t *w, int module_id);

int nod_module_alloc(nod_module_t *module);
int nod_module_load(nod_module_t *module);
int nod_module_run(nod_module_t *module, int runnable);
int nod_module_remove(nod_module_t *module);
int nod_module_free(nod_module_t *module);
int nod_module_init(nod_module_t *module);
int nod_module_stop(nod_module_t *module);
void nod_module_kill_status_task(nod_module_t *module);

variable_t* nod_module_variable_get(nod_module_t *module, string name);
variable_t* nod_module_variable_create(nod_module_t *module, string name);

int nod_module_execinfo_add_sample(execinfo_t *execinfo, int cpu, int relinquish);

int nod_variable_init(variable_t *variable, int size);
int nod_variable_close(variable_t *variable);

int nod_waveform_serialize(nod_waveform_t *dest, packet_t *pkt,
		int all_module, enum variable_serialize_data copy_data);
int nod_waveform_unserializeTo(packet_t *pkt, nod_waveform_t *dest);



#endif
