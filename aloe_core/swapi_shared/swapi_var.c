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

#include <stddef.h>
#include "swapi.h"
#include "swapi_context.h"
#include "nod_waveform.h"

/**
 * Declares a module variable as "public". Then other modules or the manager can view and modify it.
 *
 * 1) Find if variable name exists in mymodule.variables, otherwise find an empty space and
 * fill attributes
 * 2) call hwapi.openShm() to create a shared memory area for the variable. Save pointer in
 * the variable object found in previous step
 * 3) If all ok, return the shared memory pointer, otherwise return null
 */
var_t swapi_var_create(void *context, string name, int size, void **ptr) {
	swapi_context_t *ctx = context;
	nod_module_t *module = ctx->module;

	SWAPI_ASSERT_PARAM_P(module);
	SWAPI_ASSERT_PARAM_P(name);
	SWAPI_ASSERT_PARAM_P(size>0);
	SWAPI_ASSERT_PARAM_P(ptr);
	SWAPI_ASSERT_PARAM_P(*ptr);

	variable_t *variable;
	variable = nod_module_variable_get(module, name);
	if (!variable) {
		variable = nod_module_variable_create(module, name);
		if (!variable) {
			return NULL;
		}
	}

	if (nod_variable_init(variable, size)) {
		return NULL;
	}

	*ptr = variable->cur_value;
	return 0;
}

/**
 * Sets the initialization value of a variable to the buffer pointed by ptr
 *
 * 1) find a variable in mymodule.variables with mymodule.variables[i].name=name
 * 2) copy min(size,mymodule.variables[i].size) bytes of mymodule.variables[i].initValue to ptr
 */
int swapi_var_initialize(void *context, string name, void* ptr, int size) {
	cast(ctx,context);
	SWAPI_ASSERT_PARAM(name);
	SWAPI_ASSERT_PARAM(ptr);
	SWAPI_ASSERT_PARAM(size>0);
	int cpy_sz;
	variable_t *variable = nod_module_variable_get((nod_module_t*) ctx->module, name);
	if (!variable) {
		SWAPI_SETERROR(SWAPI_ERROR_NOTFOUND);
		return -1;
	}
	cpy_sz = (variable->size > size)?size:variable->size;
	memcpy(ptr, variable->init_value, (size_t) cpy_sz);
	return cpy_sz;
}

/**
 * Declares a public variable "private" so that any other module or the manager can modify it
 *
 * 1) Find a variable in mymodule.variables with mymodule.variables[i].shmPtr=ptr
 * 2) clear mymodule.variables[i] attributes
 * 3) call hwapi.closeShm(ptr);
 */
int swapi_var_close(void *context, var_t var) {
	cast(ctx,context);
	variable_t *variable = (variable_t*) var;
	SWAPI_ASSERT_PARAM(var);

	if (!nod_variable_close(variable)) {
		SWAPI_SETERROR(SWAPI_ERROR_NOTFOUND);
		return -1;
	}
	variable->id = 0;
	return 0;
}
