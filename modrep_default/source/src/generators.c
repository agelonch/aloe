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
#include <complex.h>

#define INCLUDE_DEFS_ONLY
#include "source.h"


#define DIV(a,b) ((a-1)/b+1)

static int tmp_random[DIV(OUTPUT_MAX_SAMPLES,sizeof(int)*8)];
static char tmp_binary[OUTPUT_MAX_SAMPLES];

#define BPSK_SYMB 0.707

void random_sequence(int *x, int len) {
	int i;
	if (len > OUTPUT_MAX_SAMPLES/8) {
		len = OUTPUT_MAX_SAMPLES/8;
	}
	for (i=0;i<len;i++) {
		x[i] = i;//rand();
	}
}

void random_bits(char *output, int len) {
	int j,b,k,tmp;
	random_sequence(tmp_random,DIV(len,sizeof(int)*8));
	b=k=0;
	for (j=0;j<len;j++) {
		if (!b) {
			tmp = tmp_random[k];
			b = sizeof(int)*8;
			k++;
		}
		if (tmp & 0x1) {
			output[j] = 1;
		} else {
			output[j] = 0;
		}
		tmp>>=1;
		b--;
	}
}

int work_binary(void **data, int block_size) {
	int i,j;
	char *output;
	random_bits(tmp_binary,block_size);
	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				output[j] = tmp_binary[j];
			}
		}
	}
	return block_size;
}

int work_ramp_re(void **data, int block_size) {
	int i,j;
	float *output;

	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				output[j] = i;
			}
		}
	}
	return block_size*sizeof(float);
}

int work_ramp_c(void **data, int block_size) {
	int i,j;
	_Complex float *output;

	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				__real__ output[j] = j;
				__imag__ output[j] = block_size-j-1;
			}
		}
	}
	return block_size*sizeof(_Complex float);

}

int work_bpsk_re(void **data, int block_size) {
	int i,j;
	float *output;
	random_bits(tmp_binary,block_size);
	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				if (tmp_binary[j]) {
					output[j] = BPSK_SYMB;
				} else {
					output[j] = -BPSK_SYMB;
				}
			}
		}
	}
	return block_size*sizeof(float);
}

int work_bpsk_c(void **data, int block_size) {
	int i,j;
	_Complex float *output;

	random_bits(tmp_binary,block_size);
	for (j=0;j<block_size;j++) {
			for (i=0;i<NOF_OUTPUT_ITF;i++) {
				if (data[i]) {
					output = data[i];
					if (tmp_binary[j]) {
						__real__ output[j] = BPSK_SYMB;
					} else {
						__real__ output[j] = -BPSK_SYMB;
					}
				}
			}
		}
	random_bits(tmp_binary,block_size);
	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				if (tmp_binary[j]) {
					__imag__ output[j] = BPSK_SYMB;
				} else {
					__imag__ output[j] = -BPSK_SYMB;
				}
			}
		}
	}
	return block_size*sizeof(_Complex float);
}

int work_sin_re(void **data, int block_size) {
	int i,j;
	float *output;
	int cnt=0;
	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				switch(cnt) {
				case 0:
					output[j] = 1;
				break;
				case 1:
					output[j] = 0;
				break;
				case 2:
					output[j] = -1;
				break;
				case 3:
					output[j] = 0;
				break;
				}
				cnt++;
				if (cnt == 4) {
					cnt = 0;
				}
			}
		}
	}
	return block_size*sizeof(float);
}

int work_sin_c(void **data, int block_size) {
	int i,j;
	_Complex float *output;
	int cnt=0;
	for (j=0;j<block_size;j++) {
		for (i=0;i<NOF_OUTPUT_ITF;i++) {
			if (data[i]) {
				output = data[i];
				switch(cnt) {
				case 0:
					__real__ output[j] = 1;
					__imag__ output[j] = 0;
				break;
				case 1:
					__real__ output[j] = 0;
					__imag__ output[j] = 1;
				break;
				case 2:
					__real__ output[j] = -1;
					__imag__ output[j] = 0;
				break;
				case 3:
					__real__ output[j] = 0;
					__imag__ output[j] = -1;
				break;
				}
				cnt++;
				if (cnt == 4) {
					cnt = 0;
				}
			}
		}
	}
	return block_size*sizeof(_Complex float);
}
