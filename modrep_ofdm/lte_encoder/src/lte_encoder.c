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
#include <oesr.h>
#include <params.h>
#include <skeleton.h>

#include "lte_encoder.h"
#include "turbocoder.h"

pmid_t padding_id;
static int padding;

int initialize() {

	padding_id = param_id("padding");
	if (!padding_id) {
		modinfo("Parameter padding not configured. Setting to 0\n");
		padding = 0;
	}
	return 0;
}


int work(void **inp, void **out) {
	int i, out_len, j;
	input_t *input;
	output_t *output;

	for (i=0;i<NOF_INPUT_ITF;i++) {
		input = inp[i];
		output = out[i];
		if (get_input_samples(i)) {
			out_len = RATE*get_input_samples(i)+TOTALTAIL;
			turbo_coder(input,output,get_input_samples(i));
			for (j=0;j<padding;j++) {
				output[out_len+j] = 0;
			}
			set_output_samples(i,out_len+padding);
		}
	}
	return 0;
}

int stop() {
	return 0;
}

