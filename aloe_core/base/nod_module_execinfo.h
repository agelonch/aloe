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

#ifndef NOD_MODULE_EXECINFO_H
#define NOD_MODULE_EXECINFO_H

#include "modexecinfo.h"
#include "packet.h"

/**
 * The class attributes save information relative to the execution of the module.
 */
typedef struct  {
	execinfo_t parent;
} nod_module_execinfo_t;

int add_sample(nod_module_execinfo_t *execinfo, int cpu, int relinquish);
void nod_module_execinfo_serialize(nod_module_execinfo_t *dest, packet_t *pkt);
void nod_module_execinfo_unserializeTo(packet_t *pkt, nod_module_execinfo_t *dest);

#endif
