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

/**
 * @defgroup plp_sink plp_sink
 *
 * This sink uses plplot to plot the received samples. It supports different modes:
 * do nothing, print samples to stdout, plot received samples or plot the power spectral density of the
 * received samples. The PSD is estimated using the DFT-based periodogram.
 *
 * If multiple interfaces are defined, it plots them all in the same window.
 *
 * The PLplot interface supports many output drivers. The supported drivers depends on each
 * particular system. The driver may be selected using the PL_DRIVER constant in the plp_sink.h file.
 * Please check with plplot documentation for more information.
 *
 * The parameter EXEC_MIN_INTERVAL_MS in plp_sink.h selects the signal print/plot period, in miliseconds.
 *
 * @{
 */

#ifndef DEFINE_H
#define DEFINE_H


typedef float input_t;
typedef float output_t;

#define INPUT_MAX_SAMPLES 	14*2048
#define OUTPUT_MAX_SAMPLES  0

#define NOF_INPUT_ITF		1
#define NOF_OUTPUT_ITF		0

#endif

/**@} */

#define FFT_DEFAULT_SZ 128

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)


#define PL_DRIVER "xcairo"		/** this is the prettier, but slower and memory leakage */
//#define PL_DRIVER "xwin"		/** this is the fastest */

#define EXEC_MIN_INTERVAL_MS	100

#define MODE_SILENT		0
#define MODE_PRINT		1
#define MODE_SCOPE		2
#define MODE_PSD		3



/********* do not need to modify beyond here */


#ifndef INCLUDE_DEFS_ONLY

/* Input and output buffer sizes (in number of samples) */
const int input_max_samples = INPUT_MAX_SAMPLES;
const int output_max_samples = OUTPUT_MAX_SAMPLES;

/* leave these two lines unmodified */
const int input_sample_sz = sizeof(input_t);
const int output_sample_sz = sizeof(output_t);

/* Number of I/O interfaces. All have the same maximum size */
const int nof_input_itf = NOF_INPUT_ITF;
const int nof_output_itf = NOF_OUTPUT_ITF;

#endif
