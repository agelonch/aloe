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

#ifndef NOD_MODULE_H
#define NOD_MODULE_H

#include "nod_module_execinfo.h"
#include "nod_variable.h"
#include "interface.h"
#include "objects_max.h"
#include "hwapi.h"
#include "swapi_context.h"
#include "module.h"

/**
 * Stores the attributes of waveform module.
 */
typedef struct {
	module_t parent;
	nod_module_execinfo_t execinfo;
	swapi_context_t swapi_context;
	h_proc_t *process;
	interface_t interfaces[MAX(nod_interfaces)];
	nod_variable_t variables[MAX(nod_variables)];
} nod_module_t;

void nod_module_serialize(nod_module_t *src, packet_t *pkt);
void nod_module_unserializeTo(packet_t *pkt, nod_module_t *dest);

#endif
