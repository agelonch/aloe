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

#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <string.h>

#define INCLUDE_DEFS_ONLY
#include "plp_sink.h"

static int fft_size;
static int is_complex;
static double norm;

fftwf_complex *c_in_fft, *c_out_fft;
fftwf_plan c_plan;

fftw_plan r_plan;
double *r_in_fft, *r_out_fft;

int fft_init(int _size, int _is_complex) {
	fft_size = _size;
	is_complex = _is_complex;

	if (!fft_size) {
		return -1;
	}

	if (is_complex) {
		c_in_fft = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*fft_size);
		c_out_fft = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*fft_size);
		c_plan = fftwf_plan_dft_1d(fft_size, c_in_fft, c_out_fft, -1, 0);
		if (!c_plan) {
			return -1;
		}
	} else {
		r_in_fft = (double*) fftw_malloc(sizeof(double)*fft_size);
		r_out_fft = (double*) fftw_malloc(sizeof(double)*fft_size);
		r_plan = fftw_plan_r2r_1d(fft_size, r_in_fft, r_out_fft,
				FFTW_R2HC,0);
		if (!r_plan) {
			return -1;
		}
	}

	/* Normalizatize DFT/IDFT output*/
	norm = 1/sqrt(fft_size);

	return 0;
}

int c_fft_execute(void **inp, double *pl_signals, int *signal_lengths) {
	int i,j,len;
	memset(c_in_fft,0,sizeof(_Complex float)*fft_size);

	for (i=0;i<NOF_INPUT_ITF;i++) {
		len = MIN(signal_lengths[i],fft_size);
		if (inp[i]) {
			memcpy(c_in_fft,inp[i],len*sizeof(_Complex float));

			fftwf_execute(c_plan);

			for (j=0;j<len;j++) {
				c_out_fft[j] /= fft_size;
				pl_signals[i*INPUT_MAX_SAMPLES+j] =
						10*log10((__real__ c_out_fft[j])*(__real__ c_out_fft[j])+
						(__imag__ c_out_fft[j])*(__imag__ c_out_fft[j]));
				if (isinf(-pl_signals[i*INPUT_MAX_SAMPLES+j])) {
					pl_signals[i*INPUT_MAX_SAMPLES+j] = -60;
				}
			}
		} else {
			memset(&pl_signals[i*INPUT_MAX_SAMPLES],0,sizeof(double)*len);
		}
	}
}


int r_fft_execute(void **inp, double *pl_signals, int *signal_lengths) {
	int i,j,len;
	float *input;

	for (i=0;i<NOF_INPUT_ITF;i++) {
		len = MIN(signal_lengths[i],fft_size);
		if (inp[i]) {
			input = inp[i];
			for (j=0;j<len;j++) {
				r_in_fft[j] = (double) input[j];
			}

			fftw_execute(r_plan);

			pl_signals[0] = r_out_fft[0]*r_out_fft[0];
			for (j=0;j<(len+1)/2-1;j++) {
				r_out_fft[j] /= fft_size;
				r_out_fft[len-j-1] /= fft_size;
				pl_signals[i*INPUT_MAX_SAMPLES+j] =
						r_out_fft[j]*r_out_fft[j]+
						r_out_fft[len-j-1]*r_out_fft[len-j-1];
				pl_signals[i*INPUT_MAX_SAMPLES+j] = 10*log10(pl_signals[i*INPUT_MAX_SAMPLES+j]);
				if (isinf(-pl_signals[i*INPUT_MAX_SAMPLES+j])) {
					pl_signals[i*INPUT_MAX_SAMPLES+j] = -30;
				}
			}
			if (len % 2 == 0) {
				pl_signals[i*INPUT_MAX_SAMPLES+len/2-1] = 20*log10(r_out_fft[len/2]);
				if (isinf(-pl_signals[i*INPUT_MAX_SAMPLES+len/2-1])) {
					pl_signals[i*INPUT_MAX_SAMPLES+len/2-1] = -30;
				}
			}
		} else {
			memset(&pl_signals[i*INPUT_MAX_SAMPLES],0,sizeof(double)*len);
		}
	}
}

int fft_execute(void **inp, double *pl_signals, int *signal_lengths) {
	int i,j,len;
	float *input;

	if (is_complex) {
		c_fft_execute(inp,pl_signals,signal_lengths);
	} else {
		r_fft_execute(inp,pl_signals,signal_lengths);
	}

	return 0;
}

void fft_destroy() {
	if (is_complex) {
		fftwf_destroy_plan(c_plan);
		fftwf_free(c_in_fft);
		fftwf_free(c_out_fft);
	} else {
		fftw_free(r_in_fft);
		fftw_free(r_out_fft);
		fftw_destroy_plan(r_plan);
	}
}



