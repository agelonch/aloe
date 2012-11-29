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

#include <string.h>
#include "mex.h"
/*#include "matrix.h"*/
#define SKELETON_H
#include "../aloe/params.h"
#define INTERFACE_CONFIG
#include "../src/modulename_interfaces.h"
#undef INTERFACE_CONFIG
#include "../src/modulename_params.h"
#include "../src/modulename.h"

#define IN 	prhs[0]
#define CTRL	prhs[1]
#define	OUT	plhs[0]

#if !defined(MAX)
#define	MAX(A, B)	((A) > (B) ? (A) : (B))
#endif

#if !defined(MIN)
#define	MIN(A, B)	((A) < (B) ? (A) : (B))
#endif

static int *input_len;
static int *output_len;
static int *array_dims;

input_t *input_buffer;
output_t *output_buffer;

int *iscomplex;

static int output_is_complex;

int get_input_samples(int idx) {
	return input_len[idx];
}
void set_output_samples(int idx, int len) {
	output_len[idx] = len;
}

int get_input_max_samples() {
	return input_max_samples;
}


void help() {
#if nof_input_itfS > 1
	mexErrMsgTxt("First parameter is a cell array where each element is a vector of samples "
					"for each input interface. \nSecond parameter is a cell array where each element configures"
					"a different control parameter. These elements have two fields: a string and a numeric value.\n "
	#if nof_output_itfS > 1
				"The function returns a cell array"
				"where each element is a vector of samples for each output interface.\n");
	#else
				"The function returns a vector containing the samples of the output interface.\n");
	#endif
#else
	mexErrMsgTxt("First parameter is a vector containing the samples of the input interface."
					"\nSecond parameter is a cell array where each element configures"
					"a different control parameter. These elements have two fields: a string and a numeric value.\n "
	#if nof_output_itfS > 1
				"The function returns a cell array"
				"where each element is a vector of samples for each output interface.\n");
	#else
				"The function returns a vector containing the samples of the output interface.\n");
	#endif
#endif
}

int get_vector_len(const mxArray *in) {
	int m,n;
	n = mxGetN(in);
	m = mxGetM(in);
	if (MIN(m,n) != 1) {
		help();
	}
	return MAX(m,n);
}

int fill_input_lengths(const mxArray *in) {
	int n;
	int i;

	if (mxIsCell(in)) {
		n = mxGetNumberOfElements(in);
		for(i=0;i<n;i++) {
			input_len[i] = get_vector_len(mxGetCell(in,i));
			iscomplex[i] = mxIsComplex(mxGetCell(in,i));
		}
		return n;
	} else {
		iscomplex[0] = mxIsComplex(in);
		input_len[0] = get_vector_len(in);
		return 1;
	}
}

void format_input_vector(input_t *buffer, const mxArray *in, int iscomplex, int len) {
	double *in_real, *in_imag;
	int i;

	if (!mxIsDouble(in)) {
		help();
	}

	in_real = mxGetPr(in);
	if (iscomplex) {
		in_imag = mxGetPi(in);
	}

	if (len>input_max_samples) {
		mexErrMsgTxt("Input vector too large");
	}

	for (i=0;i<len;i++) {
		__real__ buffer[i] = in_real[i];
		if (iscomplex) {
			__imag__ buffer[i] = in_imag[i];
		}
	}
}

void format_input(const mxArray *in) {
	int n;
	int i;

	if (mxIsCell(in)) {
		n = mxGetNumberOfElements(in);
		for(i=0;i<n;i++) {
			format_input_vector(&input_buffer[i*input_max_samples],mxGetCell(in,i), iscomplex[i],
					input_len[i]);
		}
	} else {
		format_input_vector(input_buffer,in, iscomplex[0], input_len[0]);
	}
}

void save_output(mxArray **dst, output_t *src, int len) {
	double *out_real, *out_imag; /* pointers to input arguments (passed from Matlab) */
	int i;

	/* Create a matrix for the return argument */
	*dst = mxCreateDoubleMatrix(1, len, output_is_complex?mxCOMPLEX:mxREAL);

    out_real = mxGetPr(*dst);
    if (output_is_complex) {
        out_imag = mxGetPi(*dst);
    }
    for (i=0;i<len;i++) {
    	out_real[i] = __real__ src[i];
    	if (output_is_complex) {
			out_imag[i] = __imag__ src[i];
		}
	}

}

void process_control(const mxArray *ctrl) {
	int nof_params;
	int i,j;
	int slen;
	char *s;
	void *pmem;
	mxArray *cellparam,*cellkey,*cellvalue;
	void *dst;
	double *src;
	int plen;
	param_t *param;

	if (!mxIsCell(ctrl)) {
		help();
	}

	param_init(parameters, nof_parameters);

	nof_params = mxGetNumberOfElements(ctrl);
	for (j=0;j<nof_params;j++) {
		cellparam = mxGetCell(ctrl,j);
		if (!mxIsCell(cellparam)) {
			help();
		}
		if (mxGetNumberOfElements(cellparam) != 2) {
			help();
		}
		cellkey = mxGetCell(cellparam,0);
		cellvalue = mxGetCell(cellparam,1);
		if (!mxIsDouble(cellvalue)) {
			mexErrMsgTxt("Only numeric parameters are supported\n");
		}
		if (!mxIsChar(cellkey)) {
			help();
		}
		slen = MAX(mxGetM(cellkey),mxGetN(cellkey))+1;
		s = mxCalloc(slen, sizeof(char));
		if (mxGetString(cellkey,s,slen)) {
			mexErrMsgTxt("Error reading string\n");
		}
		param = param_get(s);
		if (!param) {
			mexErrMsgTxt(param_error_str());
		}
		dst = param_get_addr(s);
		if (!dst) {
			mexErrMsgTxt(param_error_str());
		}
		src = mxGetPr(cellvalue);
		plen = MAX(mxGetM(cellvalue),mxGetN(cellvalue));
		switch(param->type) {
		case INT:
			for (i=0;i<plen;i++) {
				*((int*) dst+i) = (int) src[i];
			}
			break;
		case FLOAT:
			for (i=0;i<plen;i++) {
				*((float*) dst+i) = (float) src[i];
			}
			break;
		case STRING:
			mxGetString(cellvalue,dst,plen);
			break;
		default:
			mexErrMsgTxt("Parameter type not implemented");
			break;
		}
		if (mxIsComplex(cellvalue)) {
			mexErrMsgTxt("Complex params not supported\n");
		}
	}
}

void allocate_memory() {
	input_len = mxCalloc(sizeof(int),nof_input_itf);
	output_len = mxCalloc(sizeof(int),nof_output_itf);
	input_buffer = mxCalloc(input_sample_sz,nof_input_itf*input_max_samples);
	output_buffer = mxCalloc(output_sample_sz,nof_output_itf*output_max_samples);
	array_dims = mxCalloc(sizeof(int),nof_input_itf);
	iscomplex = mxCalloc(sizeof(int),nof_input_itf);
}


/* the gateway function */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {


	int nof_input_itf;

	int m,n,i;
	int len;
	int out_samples;
	mxArray *cell;

	if (nrhs != 2 && nrhs != 1) {
		help();
		return;
	}

	allocate_memory();

	for (i=0;i<nof_output_itf;i++) {
		array_dims[i]=1;
	}
	nof_input_itf = fill_input_lengths(IN);

	format_input(IN);

	if (nrhs == 2) {
		process_control(CTRL);
	}

	initialize();

    /* call the DSP function */
	out_samples = work(input_buffer,output_buffer);
	if (out_samples < 0) {
		mexErrMsgTxt("Error in module work() function\n");
	}

	/**FIXME: Find out a better way to obtain output itf type */
	output_is_complex=(output_sample_sz==sizeof(_Complex float));

	for (i=0;i<nof_output_itf;i++) {
		if (!output_len[i]) {
			output_len[i] = out_samples;
		}
	}

	if (nof_output_itf>1) {
		OUT = mxCreateCellArray(nof_output_itf,array_dims);
		for (i=0;i<nof_output_itf;i++) {
			cell = mxGetCell(OUT,i);
			save_output(&cell,&output_buffer[i*input_max_samples],output_len[i]);
		}
	} else {
		save_output(&OUT,&output_buffer[0],output_len[0]);
	}

    return;
}

