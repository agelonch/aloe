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

#include <pthread.h>
#include <stdlib.h>

#include "defs.h"
#include "str.h"
#include "hwapi.h"
#include "hwapi_error.h"

#define USE_POSIX_MEMALIGN	4

#define MEM_CHUNKS_MAX 500

struct mem_chunk {
	void *ptr;
	int code;
};

static struct mem_chunk chunks[MEM_CHUNKS_MAX];

static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * hwapi_shm_open() creates and opens a new, or opens an existing, shared
 * memory  object. It returns a pointer to the begin of the shared memory area.
 *
 * Each memory chunk is associated to a code. Two calls to the same function
 * with equal code return the same pointer.
 *
 * todo: Use an static fragmentation-free memory pool for embedded systems
 *
 * @param code Non-zero integer that identifies the memory chunk
 * @returns A pointer to the shared memory area. Null if error
 */
void* hwapi_shm_open(int code, int size) {
	int i, first_space, n;

	HWAPI_ASSERT_PARAM_P(code);

	pthread_mutex_lock( &cs_mutex );

	first_space = -1;
	/* find an empty chunk  */
	for (i=0;i<MEM_CHUNKS_MAX;i++) {
		if (first_space == -1 && chunks[i].code) {
			first_space = i;
		}
		if (chunks[i].code == code) {
			break;
		}
	}
	if (i==MEM_CHUNKS_MAX) {
		if (first_space == -1) {
			HWAPI_SETERROR(HWAPI_ERROR_NOSPACE);
			return NULL;
		} else {
			n = first_space;
		}
	}
#ifdef USE_POSIX_MEMALIGN
	int s = posix_memalign(&chunks[n].ptr, USE_POSIX_MEMALIGN, (size_t) size);
	if (s) {
		HWAPI_POSERROR(s, "memalign");
		return NULL;
	}
#else
	chunks[n].ptr = malloc((size_t) size);
#endif
	if (!chunks[n].ptr) {
		HWAPI_SYSERROR("malloc");
		return NULL;
	}
	chunks[n].code = code;

	pthread_mutex_unlock( &cs_mutex );

	return chunks[n].ptr;
}

/**
 * The hwapi_shm_close() function performs the converse operation of hwapi_shm_open,
 * removing an object previously created by hwapi_shm_open().
 * The memory pointed by \param ptr can not be used any more, as it may contain
 * data reserved to other purposes.
 *
 * @returns 0 if ok, -1 if error
 */
int hwapi_shm_close(int code) {
	int i;

	pthread_mutex_lock( &cs_mutex );

	for (i=0;i<MEM_CHUNKS_MAX;i++) {
		if (chunks[i].code == code) {
			break;
		}
	}
	if (i == MEM_CHUNKS_MAX) {
		HWAPI_SETERROR(HWAPI_ERROR_NOTFOUND);
		return -1;
	}

	free(chunks[i].ptr);
	chunks[i].ptr = NULL;
	chunks[i].code = 0;

	pthread_mutex_unlock( &cs_mutex );

	return 0;
}
