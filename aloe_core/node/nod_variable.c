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

#include "defs.h"
#include "packet.h"
#include "hwapi.h"
#include "nod_waveform.h"
#include "mempool.h"
#include "swapi_context.h"


/** \brief Allocates memory for the variable value. Since all memory is shared between threads,
 * the memory is allocated using the common mempool. Allocates size bytes.
 * This function is called from the swapi when a new module creates a new variables.
 */
int nod_variable_init(variable_t *variable, int size) {
	ndebug("variable_id=%d, size=%d, current_size=%d\n",variable->id,variable->size,size);
	if (!variable) {
		return -1;
	}
	if (size<0) {
		return -1;
	}
	if (variable->cur_value) {
		return -1;
	}
	variable->cur_value = pool_alloc(size, 1);
	if (!variable->cur_value) {
		return -1;
	}
	variable->size = size;
	return 0;
}

/** \brief Deallocates the memory allocated by nod_variable_init()
 */
int nod_variable_close(variable_t *variable) {
	ndebug("variable_id=%d, size=%d\n",variable->id,variable->size);
	if (!variable) {
		return -1;
	}
	if (variable->cur_value == NULL) {
		return -1;
	}
	if (pool_free(variable->cur_value)) {
		return -1;
	}
	variable->id = 0;
	variable->size = 0;
	return 0;
}



