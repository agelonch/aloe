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

#include <stdio.h>

#include <swapi.h>
#include <skeleton.h>
#include <params.h>

#include "source.h"
#include "generators.h"

pmid_t blen_id, gen_id;
int cnt=0;
static int last_type;
static int last_block_length;


int initialize() {
	int size;
	int block_length;

	blen_id = param_id("block_length");
	if (!blen_id) {
		moderror("Parameter block_length not found\n");
		return -1;
	}

	if (param_get_int(blen_id,&block_length) != 1) {
		moderror("Getting integer parameter block_length\n");
		return -1;
	}

	modinfo_msg("Parameter block_length is %d\n",block_length);

	gen_id = param_id("generator");
	if (!gen_id) {
		moderror("Parameter type not found\n");
		return -1;
	}

	last_type = -1;

	return 0;
}

int work(void **inp, void **out) {
	int block_length, type;
	int i,j;
	int snd_samples;

	block_length = 0;
	if (param_get_int(blen_id,&block_length) != 1) {
		moderror("Getting integer parameter block_length\n");
		return -1;
	}
	type = 0;
	if (param_get_int(gen_id,&type) != 1) {
		moderror("Getting integer parameter type\n");
		return -1;
	}

	for(i=0;i<NOF_GENERATORS;i++) {
		if (generators[i].key == type) {
			break;
		}
	}
	if (i == NOF_GENERATORS) {
		moderror_msg("Generator type %d not implemented\n", type);
		return -1;
	}
	if (type != last_type) {
		modinfo_msg("Select generator: %s\n",generators[i].desc);
		last_type = type;
	}

	if (block_length > OUTPUT_MAX_SAMPLES/generators[i].samp_sz) {
		moderror_msg("block_length %d too large. Maximum is %d for this generator\n",block_length,
				OUTPUT_MAX_SAMPLES/generators[i].samp_sz);
		return -1;
	}

	if (block_length != last_block_length) {
		modinfo_msg("Select block_length: block_length=%d\n",block_length);
		last_block_length = block_length;
	}

	snd_samples = generators[i].work(out,block_length);

	return snd_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}



