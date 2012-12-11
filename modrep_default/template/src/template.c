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

#include "template.h"

pmid_t gain_id;
int block_length;

/**
 * @ingroup template
 *  Document here the module's initialization parameters
 *
 * The documentation should explain which are the possible parameters, what they do and if they are
 * mandatory or optional (indicating the default value in such case).
 *
 * \param gain Document paramater gain
 * \param block_length Document parameter block_length
 *
 * \returns This function returns 0 on success or -1 on error
 */
int initialize() {
	int size;

	/* obtains a handler for fast access to the parameter */
	gain_id = param_id("gain");
	/* In this case, we are obtaining the parameter value directly */
	if (param_get_int_name("block_length", &block_length)) {
		block_length = 0;
	}
	/* use this function to print formatted messages */modinfo_msg("Parameter block_length is %d\n",block_length);

	/* Verify control parameters */
	if (block_length > input_max_samples || block_length < 0) {
		moderror_msg("Invalid block length %d\n", block_length);
		return -1;
	}

	/* here you may do some other initialization stuff */

	return 0;
}

/**
 * @ingroup template
 *
 *  Main DSP function
 *
 * Document here your module, which parameters it requires and how it behaves as a function of them.
 *
 * \param inp Input interface buffers. The value inp[i] points to the buffer received
 * from the i-th interface. The function get_input_samples(i) returns the number of received
 * samples (the sample size is by default sizeof(input_t))
 *
 * \param out Output interface buffers. The value out[i] points to the buffer where the samples
 * to be sent through the i-th interfaces must be stored.
 *
 * @return On success, returns a non-negative number indicating the output
 * samples that should be transmitted through all output interface. To specify a different length
 * for certain interface, use the function set_output_samples(int idx, int len)
 * On error returns -1.
 *
 *
 */
int work(void **inp, void **out) {
	int rcv_samples, snd_samples;
	int j;
	input_t *input;
	output_t *output;
	float gain;

	if (param_get_float(gain_id, &gain) != 1) {
		moderror("Error getting parameter gain\n");
		return -1;
	}

	/* inp[n] and out[m] are pointer to the n-th and m-th input and output interfaces */
	/* let us assume we only have one input and output interface */
	input = inp[0];
	output = out[0];
	rcv_samples = get_input_samples(0); /* this function returns the samples received from an input */
	for (j = 0; j < rcv_samples; j++) {
		/* do here your DSP work */
		output[j] = gain*input[j];
	}
	snd_samples = rcv_samples;
	return snd_samples;
}

/**  Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}

