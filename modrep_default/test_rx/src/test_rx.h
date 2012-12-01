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

/* Constants and function prototyes directly used by the DSP modules being implemented */

#define PI		3.14159265359	/* pi approximation */
#define PIx2		6.28318530718

#define SIZE		1000000


/* input and output data types */
typedef _Complex float input_t;
typedef _Complex float output_t;

#ifdef MODULE_CONFIG

/* Input and output buffer sizes (in number of samples) */
const int input_max_samples = 14*2048;
const int output_max_samples = 0;

/* leave these two lines unmodified */
const int input_sample_sz = sizeof(input_t);
const int output_sample_sz = sizeof(output_t);

/* Number of I/O interfaces. All have the same maximum size */
const int nof_input_itf = 1;
const int nof_output_itf = 0;



/* Configure the control variables */

param_t parameters[] = {
		{"block_length", 	/* Parameter name*/
		  INT,			/* Parameter type (see skeleton.h) */
		  1},			/* Number of samples (of size the type) */
		{NULL, 0, 0} /* need to end with null name */
};

/* This number MUST be equal or greater than the number of parameters*/
const int nof_parameters = 2;

#endif
