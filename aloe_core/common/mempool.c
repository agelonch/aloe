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
#include "mempool.h"

/* @TODO: Implement a pool of fixed-size elements that allocates all memory at boot */

/** \brief allocates nof_elems elements of size size from a heap.
 *
 */
void *pool_alloc(int nof_elems, size_t size) {
	void *p;
	p = calloc((size_t) nof_elems,size);
	memdebug("nelem=%d, size=%d, calloc=0x%x\n",nof_elems,size,p);
	return p;
}

/** \brief re-allocates ptr to use nof_elems elements of size size.
 *
 */
void *pool_realloc(void *ptr, int nof_elems, size_t size) {
	void *p;
	p = realloc(ptr, (size_t) nof_elems*size);
	memdebug("nelem=%d, size=%d, realloc=0x%x\n",nof_elems,size,p);
	return p;
}

/** \brief de-allocates ptr from a heap.
 *
 */
int pool_free(void *ptr) {
	memdebug("ptr=0x%x\n",ptr);
	free(ptr);
	return 0;
}




