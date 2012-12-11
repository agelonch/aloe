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

#include "params.h"
#include "skeleton.h"

#define IN 	prhs[0]
#define CTRL	prhs[1]
#define	OUT	plhs[0]

#if !defined(MAX)
#define	MAX(A, B)	((A) > (B) ? (A) : (B))
#endif

#if !defined(MIN)
#define	MIN(A, B)	((A) < (B) ? (A) : (B))
#endif


extern const int input_sample_sz;
extern const int output_sample_sz;
extern const int nof_input_itf;
extern const int nof_output_itf;
extern const int input_max_samples;
extern const int output_max_samples;

static int *input_len;
static int *output_len;
static int *array_dims;

static char *input_buffer;
static char *output_buffer;
static void **input_ptr;
static void **output_ptr;

typedef struct {
	char *name;
	mxArray *value;
} mexparam_t;

mexparam_t *parameters;
int nof_params;

int get_input_samples(int idx) {
	if (idx<0 || idx>nof_input_itf)
		return -1;
	return input_len[idx];
}
int set_output_samples(int idx, int len) {
	if (idx<0 || idx>nof_input_itf)
			return -1;
	output_len[idx] = len;
	return 0;
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
		}
		return n;
	} else {
		input_len[0] = get_vector_len(in);
		return 1;
	}
}

void format_input_vector(void *buffer, const mxArray *in, int len) {
	double *in_real, *in_imag;
	char *in_bit;
	float *f_buffer = buffer;
	_Complex float *c_buffer = buffer;
	char *b_buffer = buffer;
	int i;

	if (len>input_max_samples) {
		mexErrMsgTxt("Input vector too large");
	}

	if (input_sample_sz == sizeof(_Complex float)) {
		if (!mxIsComplex(in)) {
			mexErrMsgTxt("Module input is complex. Matlab signal must be complex too.\n");
		}
		in_real = mxGetPr(in);
		in_imag = mxGetPi(in);
		for (i=0;i<len;i++) {
			__real__ c_buffer[i] = (float) in_real[i];
			__imag__ c_buffer[i] = (float) in_imag[i];
		}
	} else if (input_sample_sz == sizeof(float)) {
		if (!mxIsDouble(in)){
			mexErrMsgTxt("Module input is real. Matlab signal must be real too.\n");
		}
		in_real = mxGetPr(in);
		for (i=0;i<len;i++) {
			f_buffer[i] = (float) in_real[i];
		}
	} else if (input_sample_sz == sizeof(char)) {
		if (!mxIsLogical(in)) {
			mexErrMsgTxt("Module input is bitstream. Matlab signal must be logical.\n");
		}
		in_bit = (char*) mxGetData(in);
		for (i=0;i<len;i++) {
			b_buffer[i] = in_bit[i];
		}
	} else {
		mexErrMsgTxt("Matlab signal type not supported\n");
	}

}

void format_input(const mxArray *in) {
	int n;
	int i;

	if (mxIsCell(in)) {
		n = mxGetNumberOfElements(in);
		for(i=0;i<n;i++) {
			format_input_vector(&input_buffer[i*input_max_samples*input_sample_sz],mxGetCell(in,i),
					input_len[i]);
		}
	} else {
		format_input_vector(input_buffer,in, input_len[0]);
	}
}

void save_output(mxArray **dst, void *src, int len) {
	double *out_real, *out_imag;
	char *out_bit;
	int i;
	float *f_src = src;
	_Complex float *c_src = src;
	char *b_src = src;

	if (output_sample_sz == sizeof(_Complex float)) {
		*dst = mxCreateDoubleMatrix(1, len, mxCOMPLEX);
	    out_real = mxGetPr(*dst);
        out_imag = mxGetPi(*dst);
        for (i=0;i<len;i++) {
        	out_real[i] = (double) __real__ c_src[i];
			out_imag[i] = (double) __imag__ c_src[i];
    	}
	} else if (output_sample_sz == sizeof(float)) {
		*dst = mxCreateDoubleMatrix(1, len, mxREAL);
		out_real = mxGetPr(*dst);
		for (i=0;i<len;i++) {
			out_real[i] = (double) f_src[i];
		}
	} else if (output_sample_sz == sizeof(char)) {
		*dst = mxCreateLogicalMatrix(1, len);
		out_bit = (char*) mxGetData(*dst);
		for (i=0;i<len;i++) {
			out_bit[i] = b_src[i];
		}
	}
}

void parse_parameters(const mxArray *ctrl) {
	int j;
	int slen;
	mxArray *cellparam,*cellkey;

	if (!mxIsCell(ctrl)) {
		help();
	}

	nof_params = mxGetNumberOfElements(ctrl);
	parameters = mxCalloc(sizeof(mexparam_t),nof_params);

	for (j=0;j<nof_params;j++) {
		cellparam = mxGetCell(ctrl,j);
		if (!mxIsCell(cellparam)) {
			help();
		}
		if (mxGetNumberOfElements(cellparam) != 2) {
			help();
		}
		cellkey = mxGetCell(cellparam,0);
		if (!mxIsChar(cellkey)) {
			help();
		}
		slen = MAX(mxGetM(cellkey),mxGetN(cellkey))+1;
		parameters[j].name = mxCalloc(slen, sizeof(char));
		if (mxGetString(cellkey, parameters[j].name, slen)) {
			mexErrMsgTxt("Error reading string\n");
		}
		parameters[j].value  = mxGetCell(cellparam,1);
	}
}

void allocate_memory() {
	input_len = mxCalloc(sizeof(int),nof_input_itf);
	output_len = mxCalloc(sizeof(int),nof_output_itf);
	input_buffer = mxCalloc(input_sample_sz,nof_input_itf*input_max_samples);
	output_buffer = mxCalloc(output_sample_sz,nof_output_itf*output_max_samples);
	input_ptr = mxCalloc(sizeof(void*), nof_input_itf);
	output_ptr = mxCalloc(sizeof(void*), nof_output_itf);
	array_dims = mxCalloc(sizeof(int),nof_input_itf);
}


/* the gateway function */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	int i;
	int out_samples;
	mxArray *cell;

	if (nrhs != 2 && nrhs != 1) {
		help();
		return;
	}

	if (nrhs == 2) {
		parse_parameters(CTRL);
	}

	if (initialize()<0) {
		mexErrMsgTxt("Error in module initialize() function\n");
	}

	allocate_memory();

	for (i=0;i<nof_output_itf;i++) {
		array_dims[i]=1;
	}
	fill_input_lengths(IN);

	format_input(IN);


#ifndef _ALOE_OLD_SKELETON
	for (i=0;i<nof_input_itf;i++) {
		input_ptr[i] = &input_buffer[i*input_max_samples*input_sample_sz];
	}
	for (i=0;i<nof_output_itf;i++) {
		output_ptr[i] = &output_buffer[i*output_max_samples*output_sample_sz];
	}
	out_samples = work(input_ptr, output_ptr);
#else
	out_samples = work(input_buffer,output_buffer);
#endif

	if (out_samples < 0) {
		mexErrMsgTxt("Error in module work() function\n");
	}

	if (stop()<0) {
		mexErrMsgTxt("Error in module stop() function\n");
	}

	for (i=0;i<nof_output_itf;i++) {
		if (!output_len[i]) {
			output_len[i] = out_samples;
		}
	}

	if (nof_output_itf>1) {
		OUT = mxCreateCellArray(nof_output_itf,array_dims);
		for (i=0;i<nof_output_itf;i++) {
			cell = mxGetCell(OUT,i);
			save_output(&cell,&output_buffer[i*output_max_samples*output_sample_sz],output_len[i]);
		}
	} else {
		save_output(&OUT,&output_buffer[0],output_len[0]);
	}

    return;
}


pmid_t param_id(char *name) {
	int i;
	for (i=0;i<nof_params;i++) {
		if (!strcmp(name,parameters[i].name))
			break;
	}
	if (i==nof_params) {
		return NULL;
	}
	return &parameters[i];
}

int param_get(pmid_t id, void *ptr, int max_size, param_type_t *type) {

	if (!max_size) return -1;
	if (!ptr) return -1;
	if (!id) return -1;
	mexparam_t *param = (mexparam_t*) id;


	if (!mxIsNumeric(param->value)) {
		mexPrintf("Error parsing parameter name=%d. Value must be numeric\n",param->name);
		return -1;
	}

	/* parse value */

	/* try integer */
	if (mxIsInt32(param->value)) {
		*((int*) ptr) = *((int*) mxGetData(param->value));
		if (type) {
			*type = INT;
		}
		return sizeof(int);
	}

	/* try double */
	if (mxIsDouble(param->value)) {
		*((float*) ptr) = (float) *((double*) mxGetData(param->value));
		if (type) {
			*type = FLOAT;
		}
		return sizeof(float);
	}

	mexPrintf("Error: value type of parameter name %s is not supported.\n"
			"Only int32 and double numeric values are supported",param->name);
	return -1;
}



#ifdef _ALOE_OLD_SKELETON
int get_input_max_samples() {
	return input_max_samples;
}

int get_output_max_samples() {
	return output_max_samples;
}
#endif




