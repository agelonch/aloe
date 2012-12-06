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

#include <aloe/swapi_static.h>
#include <aloe/skeleton.h>
#include <aloe/params.h>
#include <aloe/str.h>

#include "plp_sink.h"
#include "plp.h"
#include "fft.h"

pmid_t mode_id, fftsize_id;

void print_signal(void *input, int len);

static char r_legends[2*NOF_INPUT_ITF][STR_LEN];
static char c_legends[2*NOF_INPUT_ITF][STR_LEN];
static char fft_legends[2*NOF_INPUT_ITF][STR_LEN];
static int signal_lengths[2*NOF_INPUT_ITF];

static double pl_signals[2*NOF_INPUT_ITF*INPUT_MAX_SAMPLES];
static int plp_initiated=0;
static int fft_initiated=0;
static int interval_ts, last_tstamp;
static int print_not_received;

static int is_complex;
static int fft_size;

void setup_legends();

int initialize() {
	int i;
	int mode;
	int tslen;

	setup_legends();
	if (param_get_int(param_id("is_complex"),&is_complex) != 1) {
		moderror("Error getting parameter is_complex\n");
		return -1;
	}

	if (param_get_int(param_id("print_not_received"),&print_not_received)!=1) {
		print_not_received = 0;
	}
	modinfo_msg("print_not_received=%d\n",print_not_received);

	mode_id = param_id("mode");
	if (mode_id == NULL) {
		modinfo("Mode is not configured, using default mode 'silent'\n");
	} else {
		if (param_get_int(mode_id,&mode) != 1) {
			moderror("Error getting parameter mode\n");
			return -1;
		}
		if (mode == MODE_SCOPE || mode == MODE_FFT) {
			modinfo("Initiating plplot...\n");
			if (plp_init(PL_DRIVER,"",is_complex)) {
				return -1;
			}
			plp_initiated = 1;
			reset_axis();
		}
	}

	fftsize_id = param_id("fft_size");
	if (fftsize_id == NULL) {
		modinfo("Warning parameter fft_size not defined. Set to default (128)\n");
		fft_size = FFT_DEFAULT_SZ;
	} else {
		if (param_get_int(fftsize_id,&fft_size) != 1) {
			moderror("Error getting parameter fft_size\n");
			return -1;
		}
	}
	if (mode == MODE_FFT) {
		if (fft_init(fft_size, is_complex)) {
			moderror_msg("Initiating FFT for size %d\n",fft_size);
			return -1;
		} else {
			modinfo_msg("Initiated FFT for size %d\n",fft_size);
		}
		fft_initiated = 1;
	}


#ifdef _COMPILE_ALOE
	tslen = swapi_tslot_length(ctx);
	if (tslen > EXEC_MIN_INTERVAL_MS*1000) {
		interval_ts = 1;
	} else {
		interval_ts = (EXEC_MIN_INTERVAL_MS*1000)/tslen;
		modinfo_msg("Timeslot is %d usec, refresh interval set to %d tslots\n",tslen,interval_ts);
	}
	last_tstamp = 0;
#endif

	return 0;
}

int work(void **inp, void **out) {
	int n,i;
	int mode;
	float *r_input;
	_Complex float *c_input;
	strdef(xlabel);

#ifdef _COMPILE_ALOE
	if (swapi_tstamp(ctx)-last_tstamp < interval_ts) {
		return 0;
	}
#endif
	last_tstamp = interval_ts;

	if (mode_id != NULL) {
		if (param_get_int(mode_id,&mode) != 1) {
			mode = 0;
		}
	} else {
		mode = 0;
	}
	memset(signal_lengths,0,sizeof(int)*2*NOF_INPUT_ITF);
	for (n=0;n<NOF_INPUT_ITF;n++) {
		if (is_complex) {
			signal_lengths[2*n] = get_input_samples(n)/2;
			signal_lengths[2*n+1] = signal_lengths[2*n];
		} else {
			signal_lengths[n] = get_input_samples(n);
		}
	}

#ifdef _COMPILE_ALOE
	if (print_not_received) {
		for (n=0;n<NOF_INPUT_ITF;n++) {
			if (MOD_DEBUG) {
				ainfo_msg("ts=%d, rcv_len=%d\n",swapi_tstamp(ctx),get_input_samples(n));
			}
			if (!get_input_samples(n)) {
				modinfo_msg("ts=%d. Data not received from interface %d\n",swapi_tstamp(ctx),n);
			}

		}
	}
#endif


	switch(mode) {
	case MODE_SILENT:
		break;
	case MODE_PRINT:
		for (n=0;n<NOF_INPUT_ITF;n++) {
			if (inp[n]) {
				print_signal(inp[n],get_input_samples(n));
			}
		}
	break;
	case MODE_SCOPE:
#ifdef _COMPILE_ALOE
		snprintf(xlabel,STR_LEN,"# sample (ts=%d)",swapi_tstamp(ctx));
#else
		snprintf(xlabel,STR_LEN,"# sample");
#endif
		if (is_complex) {
			set_legend(c_legends,2*NOF_INPUT_ITF);
		} else {
			set_legend(r_legends,NOF_INPUT_ITF);
		}
		set_labels(xlabel,"amp");

		for (n=0;n<NOF_INPUT_ITF;n++) {
			if (inp[n]) {
				if (is_complex) {
					c_input = inp[n];
					for (i=0;i<signal_lengths[2*n];i++) {
						pl_signals[2*n*INPUT_MAX_SAMPLES+i] = (double) __real__ c_input[i];
						pl_signals[(2*n+1)*INPUT_MAX_SAMPLES+i] = (double) __imag__ c_input[i];
					}
				} else {
					r_input = inp[n];
					for (i=0;i<signal_lengths[n];i++) {
						pl_signals[n*INPUT_MAX_SAMPLES+i] = (double) r_input[i];
					}
				}
			}

		}

		plp_draw(pl_signals,signal_lengths,0);
	break;
	case MODE_FFT:
#ifdef _COMPILE_ALOE
		snprintf(xlabel,STR_LEN,"freq. idx (ts=%d)",swapi_tstamp(ctx));
#else
		snprintf(xlabel,STR_LEN,"freq. idx");
#endif

		set_labels(xlabel,"PSD (dB/Hz)");

		set_legend(fft_legends,NOF_INPUT_ITF);

		fft_execute(inp,pl_signals,signal_lengths);
		for (i=0;i<NOF_INPUT_ITF;i++) {
			if (signal_lengths[i]) {
				if (!is_complex) {
					signal_lengths[i] = fft_size/2;
				} else {
					signal_lengths[i] = fft_size;
				}
			}
		}
		for (i=NOF_INPUT_ITF;i<2*NOF_INPUT_ITF;i++) {
			signal_lengths[i] = 0;
		}
		plp_draw(pl_signals,signal_lengths,0);

	break;
	default:
		moderror_msg("Unknown mode %d\n",mode);
		return -1;

	}
	return 0;
}

int stop() {
	if (plp_initiated) {
		moddebug("destoying plp %d\n",1);
		plp_destroy();
	}
	if (fft_initiated) {
		moddebug("destoying fft %d\n",1);
		fft_destroy();
	}
	return 0;
}

void print_signal(void *input, int len) {
	int i;
	_Complex float *c_in = input;
	float *r_in = input;
	for (i=0;i<len;i++) {
		if (is_complex) {
			printf("input[%d/%d]=%.2f+i*%.2f\n",i, len,__real__ c_in[i],__imag__ c_in[i]);
		} else {
			printf("input[%d/%d]=%.2f\n",i, len,r_in[i]);
		}
	}
}

void setup_legends() {
	int i;

	for (i=0;i<NOF_INPUT_ITF;i++) {
		snprintf(r_legends[i],STR_LEN,"in%d",i);
		snprintf(c_legends[2*i],STR_LEN,"real(in%d)",i);
		snprintf(c_legends[2*i+1],STR_LEN,"imag(in%d)",i);
		snprintf(fft_legends[i],STR_LEN,"abs(in%d)",i);
	}
}


