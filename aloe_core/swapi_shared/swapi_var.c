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


/**\brief Creates a new public variable with a given name. After this call size bytes of the buffer ptr may be
 * accessible by other modules or ALOE (through the MANAPI) for either read or write.
 * On success, swapi_var_create() returns a non-null handler which is passed as a first parameter
 * to swapi_var_close() to close the public variable.
 */
var_t swapi_var_create(void *context, string name, void *ptr, int size) {
	swapi_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	SWAPI_ASSERT_PARAM_P(name);
	sdebug("name=%s, size=%d, ptr=0x%x\n",name,size,ptr);
	SWAPI_ASSERT_PARAM_P(module);
	SWAPI_ASSERT_PARAM_P(size>0);
	SWAPI_ASSERT_PARAM_P(ptr);

	variable_t *variable;
	variable = nod_module_variable_get(module, name);
	if (!variable) {
		variable = nod_module_variable_create(module, name);
		if (!variable) {
			return NULL;
		}
	}
	variable->size = size;
	variable->cur_value = ptr;
	return 0;
}

/**\brief Sets up to size bytes of the buffer pointed by ptr to the value of the initialization
 * parameter with name "name".
 * On success, returns a non-negative integer indicating the number of bytes written to ptr.
 */
int swapi_var_initialize(void *context, string name, void* ptr, int size) {
	cast(ctx,context);
	SWAPI_ASSERT_PARAM(name);
	sdebug("name=%s, size=%d, ptr=0x%x\n",name,size,ptr);

	SWAPI_ASSERT_PARAM(ptr);
	SWAPI_ASSERT_PARAM(size>0);
	int cpy_sz;
	nod_module_t *module = (nod_module_t*) ctx->module;
	variable_t *variable = nod_module_variable_get(module, name);
	if (!variable) {
		SWAPI_SETERROR(SWAPI_ERROR_NOTFOUND);
		return -1;
	}
	cpy_sz = (variable->size > size)?size:variable->size;
	memcpy(ptr, variable->init_value[module->parent.cur_mode], (size_t) cpy_sz);
	return cpy_sz;
}

/**\brief Closes a variable handler. After a call to this function, ALOE can not access to
 * this variable anymore. However, the module may still use the contents of the memory address
 * passed as a parameter in the swapi_var_create() function.
 */
int swapi_var_close(void *context, var_t var) {
	cast(ctx,context);
	variable_t *variable = (variable_t*) var;
	SWAPI_ASSERT_PARAM(var);
	variable->size = 0;
	variable->id = 0;
	return 0;
}
