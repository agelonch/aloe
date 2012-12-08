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
#include <string.h>
#include <swapi.h>
#include <params.h>
#include <skeleton.h>

#include "gen_cyclic.h"

pmid_t ofdm_symbol_sz_id;
pmid_t cyclic_prefix_sz_id;
pmid_t first_cyclic_prefix_sz_id;

int initialize() {

	ofdm_symbol_sz_id = param_id("ofdm_symbol_sz");
	if (!ofdm_symbol_sz_id) {
		moderror("Parameter ofdm_symbol_sz undefined\n");
		return -1;
	}

	cyclic_prefix_sz_id = param_id("cyclic_prefix_sz");
	if (!cyclic_prefix_sz_id) {
		moderror("Parameter cyclic_prefix_sz undefined\n");
		return -1;
	}

	first_cyclic_prefix_sz_id = param_id("first_cyclic_prefix_sz");
	if (!first_cyclic_prefix_sz_id) {
		modinfo("Parameter first_cyclic_prefix_sz undefined. Assuming equal to cyclic_prefix_sz\n");
	}

	return 0;
}


int work(void **inp, void **out) {
	int i, j;
	int nof_ofdm_symb;
	int ofdm_symbol_sz, cyclic_prefix_sz, first_cyclic_prefix_sz;
	int cpy;
	input_t *input;
	output_t *output;

	if (param_get_int(ofdm_symbol_sz_id, &ofdm_symbol_sz) != 1) {
		moderror("getting parameter ofdm_symbol_sz\n");
		return -1;
	}
	if (param_get_int(cyclic_prefix_sz_id, &cyclic_prefix_sz) != 1) {
		moderror("getting parameter cyclic_prefix_sz\n");
		return -1;
	}
	if (first_cyclic_prefix_sz) {
		if (param_get_int(first_cyclic_prefix_sz_id, &first_cyclic_prefix_sz) != 1) {
			moderror("getting parameter first_cyclic_prefix_sz\n");
			return -1;
		}
	} else {
		first_cyclic_prefix_sz = cyclic_prefix_sz;
	}
	if (first_cyclic_prefix_sz > ofdm_symbol_sz || cyclic_prefix_sz > ofdm_symbol_sz) {
		moderror_msg("Error with parameters ofdm_symbol_sz=%d, cyclic_prefix_sz=%d, "
				"first_cyclic_prefix_sz=%d\n",ofdm_symbol_sz,cyclic_prefix_sz,first_cyclic_prefix_sz);
		return -1;
	}

	for (i=0;i<NOF_INPUT_ITF;i++) {
		input = inp[i];
		output = out[i];

		if (get_input_samples(i) % ofdm_symbol_sz) {
			moderror_msg("Number of input samples (%d) must be multiple of ofdm_symbol_sz (%d), in "
					"interface %d\n",get_input_samples(i),ofdm_symbol_sz,i);
			return -1;
		}

		nof_ofdm_symb = get_input_samples(i) / ofdm_symbol_sz;

		if (nof_ofdm_symb) {
			for (j=0;j<nof_ofdm_symb;j++) {
				if (!j) {
					cpy = first_cyclic_prefix_sz;
				} else {
					cpy = cyclic_prefix_sz;
				}
				memcpy(output,&input[ofdm_symbol_sz-cpy],sizeof(input_t)*cpy);
				memcpy(&output[cpy],input,sizeof(input_t)*ofdm_symbol_sz);
				input += ofdm_symbol_sz;
				output += ofdm_symbol_sz+cpy;
			}

			set_output_samples(i, ofdm_symbol_sz + first_cyclic_prefix_sz +
								 (ofdm_symbol_sz + cyclic_prefix_sz) * (nof_ofdm_symb - 1));
		}
	}
	return 0;
}

int stop() {
	return 0;
}

