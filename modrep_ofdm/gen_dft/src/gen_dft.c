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
#include <oesr.h>
#include <params.h>
#include <skeleton.h>

#include "dft.h"
#include "gen_dft.h"

/** List of dft lengths (dft points) for which dft plans are precomputed during init */
const int precomputed_dft_len[] = {128,256,512,1024,2048};
#define NOF_PRECOMPUTED_DFT 5

#define MAX_EXTRA_PLANS	5


pmid_t dft_size_id;

dft_plan_t plans[NOF_PRECOMPUTED_DFT];
dft_plan_t extra_plans[MAX_EXTRA_PLANS];
static int direction;
static int options;

/**@ingroup gen_dft
 * \param direction Direction of the dft: 0 computes a dft and 1 computes an idft (default is 0)
 * \param mirror 0 computes a normal dft, 1 swaps the two halfes of the input signal before computing
 * the dft (used in LTE) (default is 0)
 * \param psd Set to 1 to compute the power spectral density (untested) (default is 0)
 * \param out_db Set to 1 to produce the output results in dB (untested) (default is 0)
 * \param dft_size Number of DFT points. This parameter is mandatory.
 */
int initialize() {
	int tmp;
	int i;

	memset(plans,0,sizeof(dft_plan_t)*NOF_PRECOMPUTED_DFT);
	memset(extra_plans,0,sizeof(dft_plan_t)*MAX_EXTRA_PLANS);

	if (param_get_int(param_id("direction"),&direction) != 1) {
		modinfo("Parameter direction not defined. Setting to FORWARD\n");
		direction = 0;
	}

	options = 0;
	if (param_get_int(param_id("mirror"),&tmp) != 1) {
		modinfo("Parameter mirror not defined. Disabling. \n");
	} else {
		if (tmp) options |= DFT_MIRROR;
	}
	if (param_get_int(param_id("psd"),&tmp) != 1) {
		modinfo("Parameter psd not defined. Disabling. \n");
	} else {
		if (tmp) options |= DFT_PSD;
	}
	if (param_get_int(param_id("out_db"),&tmp) != 1) {
		modinfo("Parameter out_db not defined. Disabling. \n");
	} else {
		if (tmp) options |= DFT_OUT_DB;
	}
	if (param_get_int(param_id("normalize"),&tmp) != 1) {
		modinfo("Parameter normalize not defined. Disabling. \n");
	} else {
		if (tmp) options |= DFT_NORMALIZE;
	}

	dft_size_id = param_id("dft_size");
	if (!dft_size_id) {
		moderror("Parameter dft_size not defined\n");
		return -1;
	}

	if (dft_plan_multi_c2c(precomputed_dft_len, (!direction)?FORWARD:BACKWARD, NOF_PRECOMPUTED_DFT,
			plans)) {
		moderror("Precomputing plans\n");
		return -1;
	}
	for (i=0;i<NOF_PRECOMPUTED_DFT;i++) {
		plans[i].options = options;
	}

	return 0;
}

dft_plan_t* find_plan(int dft_size) {
	int i;
	for (i=0;i<NOF_PRECOMPUTED_DFT;i++) {
		if (plans[i].size == dft_size) {
			return &plans[i];
		}
	}
	return NULL;
}

dft_plan_t* generate_new_plan(int dft_size) {
	int i;

	modinfo_msg("Warning, no plan was precomputed for size %d. Generating.\n",dft_size);
	for (i=0;i<MAX_EXTRA_PLANS;i++) {
		if (!extra_plans[i].size) {
			if (dft_plan_c2c(dft_size, (!direction)?FORWARD:BACKWARD, &extra_plans[i])) {
				return NULL;
			}
			extra_plans[i].options = options;
			return &extra_plans[i];
		}
	}
	return NULL;
}


int work(void **inp, void **out) {
	int i, j, nof_fft;
	int dft_size;
	input_t *input;
	output_t *output;
	dft_plan_t *plan;

	if (param_get_int(dft_size_id,&dft_size) != 1) {
		moderror("Getting parameter dft_size\n");
		return -1;
	}

	plan = find_plan(dft_size);
	if (!plan) {
		if ((plan = generate_new_plan(dft_size)) == NULL) {
			moderror("Generating plan.\n");
			return -1;
		}
	}

	for (i=0;i<NOF_INPUT_ITF;i++) {
		input = inp[i];
		output = out[i];

		if (get_input_samples(i) % dft_size) {
			moderror_msg("Number of input samples (%d) must be multiple of dft_size (%d), in "
					"interface %d\n",get_input_samples(i),dft_size,i);
			return -1;
		}

		nof_fft = get_input_samples(i)/dft_size;

		for (j=0;j<nof_fft;j++) {
			dft_run_c2c(plan, &input[j*dft_size], &output[j*dft_size]);
		}

		set_output_samples(i,dft_size*nof_fft);
	}
	return 0;
}

int stop() {
	dft_plan_free_vector(plans, NOF_PRECOMPUTED_DFT);
	dft_plan_free_vector(extra_plans, MAX_EXTRA_PLANS);
	return 0;
}

