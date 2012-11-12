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

#ifndef MANAPI_MODULE_H_
#define MANAPI_MODULE_H_

#include "str.h"
#include "manapi_variable.h"
#include "manapi_execinfo.h"
#include "manapi_interface.h"

/**
 * Inherits from General.Module class. Part of the CMDAPI, used to update module's variables or execinfo information
 */
typedef struct {

	variable_t variables[MAX(variables)];
	interface_t interfaces[MAX(interfaces)];
	execinfo_t execinfo;
	void *node;

	int id;
	int last_update_ts;
	strdef(name);

	/**
	 * name of the binary, not the full path. Depending on the platform it can be a stand-alone executable (program) or a library which is dynamically loaded.
	 */
	strdef(binary);

	/**
	 * Module processing requirements in MOPTS
	 */
	float c_mopts;

	/**
	 * Processor id where the module has been mapped.
	 */
	int processor_idx;
	int status;
	int nof_reporting_vars;
	int nof_variables;
	int nof_interfaces;

} module_t;

#endif
