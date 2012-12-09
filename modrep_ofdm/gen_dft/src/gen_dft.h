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
 * Computes the Discrete Fourier Transform (DFT) of the received signal. If the DFT size is
 * a power of 2, the FFT algorithm is automatically employed.
 *
 * The number of received samples must be multiple of the dft_size parameter. gen_dft then
 * computes received_samples/dft_size DFTs appending the result one after another.
 *
 * The module uses the fftw3 library. In order to increase the performance, it precomputes the
 * plan during the initialization phase for a set of dft_sizes configured in the file dft.h.
 * If eventually the dft_size parameter does not equal any of the precomputed sizes, a new plan is
 * computed at run-time. Note that a real-time failure may be probable produced as a consequence.
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
