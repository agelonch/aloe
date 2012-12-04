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

#include "str.h"

#define GENERATOR_BINARY	0
#define GENERATOR_RAMP_RE	1
#define GENERATOR_RAMP_C	2
#define GENERATOR_BPSK_RE	3
#define GENERATOR_BPSK_C	4
#define GENERATOR_SIN_RE	5
#define GENERATOR_SIN_C		6

int work_binary(void **data, int block_size);
int work_ramp_re(void **data, int block_size);
int work_ramp_c(void **data, int block_size);
int work_bpsk_re(void **data, int block_size);
int work_bpsk_c(void **data, int block_size);
int work_sin_re(void **data, int block_size);
int work_sin_c(void **data, int block_size);

typedef struct {
	int key;
	int (*work)(void **data, int block_size);
	int samp_sz;
	strdef(desc);
}generator_t;

#define NOF_GENERATORS 7

generator_t generators[] = {
		{GENERATOR_BINARY,work_binary,sizeof(char), "Random binary generator (bit output)"},
		{GENERATOR_RAMP_RE,work_ramp_re,sizeof(float), "Ramp generator (real output)"},
		{GENERATOR_RAMP_C,work_ramp_c,sizeof(_Complex float), "Ramp generator (complex output)"},
		{GENERATOR_BPSK_RE,work_bpsk_re,sizeof(float), "Random BPSK generator (real output)"},
		{GENERATOR_BPSK_C,work_bpsk_c,sizeof(_Complex float), "Random BPSK generator (complex output)"},
		{GENERATOR_SIN_RE,work_sin_re,sizeof(float), "Fs/4 sinusoid generator (real output)"},
		{GENERATOR_SIN_C,work_sin_c,sizeof(_Complex float), "Fs/4 sinusoid generator (complex output)"},
};
