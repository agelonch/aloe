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

#include "gen_padding.h"

pmid_t pre_padding_id, post_padding_id, nof_packets_id;
static int pre_padding, post_padding, nof_packets;

int get_input_sample_sz(int data_type) {
	switch(data_type) {
	case DATA_TYPE_COMPLEX:
		return sizeof(_Complex float);
	case DATA_TYPE_REAL:
		return sizeof(float);
	case DATA_TYPE_BITS:
		return sizeof(char);
	}
}

int initialize() {
	int data_type;

	if (param_get_int(param_id("data_type"),&data_type) != 1) {
		modinfo("Parameter data_type undefined. Using complex data type\n");
		data_type = DATA_TYPE_COMPLEX;
	}


	input_sample_sz = get_input_sample_sz(data_type);
	output_sample_sz = input_sample_sz;
	modinfo_msg("Chosed data type %d sample_sz=%d\n",data_type, input_sample_sz);

	pre_padding_id = param_id("pre_padding");
	if (!pre_padding_id) {
		modinfo("Parameter pre_padding undefined. Adding 0 bits.\n");
		pre_padding = 0;
	}

	post_padding_id = param_id("post_padding");
	if (!post_padding_id) {
		modinfo("Parameter post_padding undefined. Adding 0 bits.\n");
		post_padding = 0;
	}

	nof_packets_id = param_id("nof_packets");
	if (!nof_packets_id) {
		modinfo("Parameter nof_packets undefined. Assuming 1 packet.\n");
		nof_packets = 1;
	}

	return 0;
}

#define inaddr(a) &input[(j*in_pkt_len+a)*input_sample_sz]
#define outaddr(a) &output[(j*out_pkt_len+a)*input_sample_sz]

int work(void **inp, void **out) {
	int i, in_pkt_len, out_pkt_len, j;

	char *input, *output;

	param_get_int(pre_padding_id, &pre_padding);
	param_get_int(post_padding_id, &post_padding);
	param_get_int(nof_packets_id, &nof_packets);

	for (i=0;i<NOF_INPUT_ITF;i++) {
		input = inp[i];
		output = out[i];

		if ((get_input_samples(i)) % nof_packets) {
			moderror_msg("Received samples (%d) should multiple of nof_packetss (%d)\n",
					get_input_samples(i), nof_packets);
			return -1;
		}

		in_pkt_len = get_input_samples(i) / nof_packets;
		out_pkt_len = in_pkt_len + pre_padding+post_padding;

		if (in_pkt_len) {
			for (j=0;j<nof_packets;j++) {
				memset(outaddr(0),0,input_sample_sz*pre_padding);
				memcpy(outaddr(pre_padding),inaddr(0),input_sample_sz*in_pkt_len);
				memset(outaddr(pre_padding+in_pkt_len),0,input_sample_sz*post_padding);
			}
			set_output_samples(i,out_pkt_len*nof_packets);
		}

	}
	return 0;
}

int stop() {
	return 0;
}

