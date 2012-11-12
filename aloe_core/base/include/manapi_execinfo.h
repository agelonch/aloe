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

#ifndef MANAPI_EXECINFO_H_
#define MANAPI_EXECINFO_H_

#include "str.h"

/**
 * Inherits from General.ModExecInfo class. Part of the CMDAPI, adds functionality to retrieve module's ExecInfo attributes
 */

typedef struct {
	int module_ts;
	int node_ts;
	int mean_exec_us;
	int max_exec_us;
	int mean_rel_us;
	int max_rel_us;
	float mean_mopts;
	float max_mopts;
	time_t t_exec[3];
	int last_update_ts;
} execinfo_t;

#endif
