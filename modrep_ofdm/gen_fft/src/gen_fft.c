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
#include <aloe/swapi.h>
#include <aloe/params.h>
#include <aloe/skeleton.h>

#include "dft.h"
#include "gen_fft.h"

/** List of dft lengths to precompute a plan during init */
const int precomputed_dft_len[] = {128,256,512,1024,2048,4096,8192};
#define NOF_PRECOMPUTED_DFT 7

#define MAX_EXTRA_PLANS	5


pmid_t fft_size_id;

dft_plan_t plans[NOF_PRECOMPUTED_DFT];
dft_plan_t extra_plans[MAX_EXTRA_PLANS];
static int direction;
static int options;

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

	fft_size_id = param_id("fft_size");
	if (!fft_size_id) {
		moderror("Parameter fft_size not defined\n");
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

dft_plan_t* find_plan(int fft_size) {
	int i;
	for (i=0;i<NOF_PRECOMPUTED_DFT;i++) {
		if (plans[i].size == fft_size) {
			return &plans[i];
		}
	}
	return NULL;
}

dft_plan_t* generate_new_plan(int fft_size) {
	int i;

	modinfo_msg("Warning, no plan was precomputed for size %d. Generating.\n",fft_size);
	for (i=0;i<MAX_EXTRA_PLANS;i++) {
		if (!extra_plans[i].size) {
			if (dft_plan_c2c(fft_size, (!direction)?FORWARD:BACKWARD, &extra_plans[i])) {
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
	int fft_size;
	input_t *input;
	output_t *output;
	dft_plan_t *plan;

	if (param_get_int(fft_size_id,&fft_size) != 1) {
		moderror("Getting parameter fft_size\n");
		return -1;
	}

	plan = find_plan(fft_size);
	if (!plan) {
		if ((plan = generate_new_plan(fft_size)) == NULL) {
			moderror("Generating plan.\n");
			return -1;
		}
	}

	for (i=0;i<NOF_INPUT_ITF;i++) {
		input = inp[i];
		output = out[i];

		if (get_input_samples(i) % fft_size) {
			moderror_msg("Number of input samples (%d) must be multiple of fft_size (%d), in "
					"interface %d\n",get_input_samples(i),fft_size,i);
			return -1;
		}

		nof_fft = get_input_samples(i)/fft_size;

		for (j=0;j<nof_fft;j++) {
			dft_run_c2c(plan, &input[j*fft_size], &output[j*fft_size]);
		}

		set_output_samples(i,fft_size*nof_fft);
	}
	return 0;
}

int stop() {
	dft_plan_free_vector(plans, NOF_PRECOMPUTED_DFT);
	dft_plan_free_vector(extra_plans, MAX_EXTRA_PLANS);
	return 0;
}

