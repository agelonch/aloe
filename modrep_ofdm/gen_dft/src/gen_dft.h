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


/** @defgroup gen_dft gen_dft
 *
 * Computes the Discrete Fourier Transform (DFT).
 *
 * The number of received samples must be an integer multiple of the dft_size parameter. gen_dft then
 * sequentially computes received_samples/dft_size DFTs, each of dft_size points, and concatenates the outputs.
 *
 * The module uses the fftw3 library for computing the dft efficiently. In order to improve the 
 * performance and enable real-time reconfiguration, it precomputes dft plans
 * during the initialization phase for the dft_sizes configured in file dft.h.
 * If the dft_size parameter does not match any of the preconfigured sizes, a new plan is
 * created at runtime. The runtime overhead of computing a new dft plan may cause real-time failures.
 *
 *
 *
 * @{
 */

typedef _Complex float input_t;
typedef _Complex float output_t;

#define OUTPUT_MAX_SAMPLES 	14*2048
#define INPUT_MAX_SAMPLES 	14*2048

#define NOF_INPUT_ITF		1
#define NOF_OUTPUT_ITF		1

/**@} */



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
