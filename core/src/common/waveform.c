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


#include <stdlib.h>

#include "defs.h"
#include "waveform.h"
#include "mempool.h"

/** \brief Returns a pointer to the first module with id the second parameter
 * \returns a non-null pointer if found or null otherwise
 */
module_t* waveform_find_module_id(waveform_t *w, int module_id) {
	mdebug("waveform=%s, module_id=%d\n",w->name, module_id);
	int i=0;
	while(i<w->nof_modules && w->modules[i].id != module_id)
		i++;
	if (i==w->nof_modules) {
		return NULL;
	}
	return &w->modules[i];
}

/** \brief Returns a pointer to the first module with name the second parameter
 * \returns a non-null pointer if found or null otherwise
 */
module_t* waveform_find_module_name(waveform_t *w, char *name) {
	mdebug("waveform=%s, obj_name=%s\n",w->name, name);
	int i=0;
	while(i<w->nof_modules && strcmp(w->modules[i].name, name))
		i++;
	if (i==w->nof_modules) {
		return NULL;
	}
	return &w->modules[i];
}

/** \brief Allocates the memory for interfaces and variables in a module.
 * \param nof_inputs Number of input interfaces to allocate
 * \param nof_outputs Number of output interfaces to allocate
 * \param nof_variables Number of variables to allocate
 * \returns 0 on success or -1 on error
 */
int module_alloc(module_t *module, int nof_inputs, int nof_outputs, int nof_variables) {
	mdebug("module_id=%d, nof_inputs=%d, nof_outputs=%d, nof_variables=%d\n",module->id,
			nof_inputs, nof_outputs, nof_variables);
	if (!module) return -1;
	if (module->inputs || module->outputs || module->variables) {
		return -1;
	}
	module->inputs = (interface_t*) pool_alloc(nof_inputs,sizeof(interface_t));
	module->outputs = (interface_t*) pool_alloc(nof_outputs,sizeof(interface_t));
	module->variables = (variable_t*) pool_alloc(nof_variables,sizeof(variable_t));
	if (!module->inputs || !module->outputs || !module->variables) {
		module_free(module);
		return -1;
	}
	module->nof_inputs = nof_inputs;
	module->nof_outputs = nof_outputs;
	module->nof_variables = nof_variables;
	return 0;
}

/** \brief De-allocates the interface/variables memory allocated using module_alloc
 *
 */
int module_free(module_t *module) {
	int i;
	mdebug("module_id=%d\n",module->id);
	if (!module) return -1;
	if (module->inputs) {
		if (pool_free(module->inputs)) return -1;
		module->inputs = NULL;
	}
	if (module->outputs) {
		if (pool_free(module->outputs)) return -1;
		module->outputs = NULL;
	}
	if (module->variables) {
		for (i=0;i<module->nof_variables;i++) {
			if (variable_free(&module->variables[i])) {
				return -1;
			}
		}
		if (pool_free(module->variables)) return -1;
		module->variables = NULL;
	}
	module->nof_inputs = 0;
	module->nof_outputs = 0;
	module->nof_variables = 0;
	return 0;
}

/** \brief Allocates memory for modules in a waveform. Does NOT call module_alloc() individually.
 * \param nof_modules Number of modules that will be allocated
 */
int waveform_alloc(waveform_t *waveform, int nof_modules) {
	mdebug("waveform_id=%d, nof_modules=%d\n",waveform->id, nof_modules);
	if (!waveform) return -1;
	if (waveform->modules) return -1;
	waveform->modules = (module_t*) pool_alloc(nof_modules, sizeof(module_t));
	if (!waveform->modules) return -1;
	waveform->nof_modules = nof_modules;
	return 0;
}

/** \brief De-allocates the memory allocated using waveform_alloc(). Does NOT call module_free()
 * individually for each module
 */
int waveform_free(waveform_t *waveform){
	mdebug("waveform_id=%d, nof_modules=%d\n",waveform->id, waveform->nof_modules);
	if (!waveform) return -1;
	if (waveform->modules) {
		if (pool_free(waveform->modules)) return -1;
		waveform->modules = NULL;
	}
	waveform->nof_modules = 0;
	return 0;
}

/** \brief Allocates memory for nof_modes initialization values in a variable. For each mode,
 * allocates variable->size bytes
 *
 */
int variable_alloc(variable_t *variable, int nof_modes) {
	mdebug("variable_id=%d, nof_modes=%d, size=%d\n",variable->id, nof_modes, variable->size);
	int i;
	if (!variable) return -1;
	for (i=0;i<nof_modes;i++) {
		if (variable->init_value[i]) return -1;
		variable->init_value[i] = pool_alloc(1,(size_t) variable->size);
		if (!variable->init_value[i]) return -1;
	}
	variable->nof_modes = nof_modes;
	return 0;
}

/** \brief Deallocates the memory of a variable allocated using variable_alloc()
 *
 */
int variable_free(variable_t *variable) {
	mdebug("variable_id=%d, nof_modes=%d, size=%d\n",variable->id, variable->nof_modes,
			variable->size);
	int i;
	if (!variable) return -1;
	for (i=0;i<variable->nof_modes;i++) {
		if (!variable->init_value[i]) return -1;
		if (pool_free(variable->init_value[i])) return -1;
	}
	return 0;
}


