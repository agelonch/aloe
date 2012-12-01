#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "swapi_types.h"
#include "swapi.h"

#include "params.h"
#include "skeleton.h"

#define MAX_INPUTS 		10
#define MAX_OUTPUTS 	10
#define MAX_VARIABLES 	50

extern const int input_sample_sz;
extern const int output_sample_sz;
extern const int nof_input_itf;
extern const int nof_output_itf;
extern const int input_max_samples;
extern const int output_max_samples;

int input_trials = 0;
#define MAX_INPUT_TRIALS 5

itf_t inputs[MAX_INPUTS], outputs[MAX_OUTPUTS];

user_var_t *user_vars;
var_t vars[MAX_VARIABLES];
int nof_vars;

param_t *user_params;
int nof_params;

log_t mlog;
counter_t counter;

pkt_t *input_pkt[MAX_INPUTS], *output_pkt[MAX_OUTPUTS];
void *input_ptr[MAX_INPUTS], *output_ptr[MAX_OUTPUTS];
int rcv_len[MAX_INPUTS], snd_len[MAX_OUTPUTS];

#ifdef _ALOE_OLD_SKELETON
char *input_buffer, *output_buffer;
#endif

void *context;

void init_memory() {
	memset(inputs,0,sizeof(itf_t)*MAX_INPUTS);
	memset(input_pkt,0,sizeof(pkt_t*)*MAX_INPUTS);
	memset(input_ptr,0,sizeof(void*)*MAX_INPUTS);
	memset(rcv_len,0,sizeof(int)*MAX_INPUTS);

	memset(outputs,0,sizeof(itf_t)*MAX_OUTPUTS);
	memset(output_pkt,0,sizeof(pkt_t*)*MAX_OUTPUTS);
	memset(output_ptr,0,sizeof(void*)*MAX_OUTPUTS);
	memset(snd_len,0,sizeof(int)*MAX_OUTPUTS);

	memset(vars,0,sizeof(var_t)*MAX_VARIABLES);

	nof_vars=0;
	mlog=NULL;
	counter=NULL;

#ifdef _ALOE_OLD_SKELETON
	input_buffer=NULL;
	output_buffer=NULL;
#endif

}

int check_configuration(void *ctx) {
	moddebug("nof_input=%d, nof_output=%d\n",nof_input_itf,nof_output_itf);

	if (nof_input_itf > MAX_INPUTS) {
		moderror_msg("Maximum number of input interfaces is %d. The module uses %d. "
				"Increase MAX_INPUTS in swapi_static/skeleton/skeleton.c and recompile ALOE\n",
				MAX_INPUTS,nof_input_itf);
		return -1;
	}
	if (nof_output_itf > MAX_OUTPUTS) {
		moderror_msg("Maximum number of output interfaces is %d. The module uses %d. "
				"Increase MAX_OUTPUTS in swapi_static/skeleton/skeleton.c and recompile ALOE\n",
				MAX_OUTPUTS,nof_output_itf);
		return -1;
	}

	user_params = param_list(&nof_params);

	user_vars = variables_list(&nof_vars);
	if (nof_vars && nof_params>0) {
		if (nof_vars > MAX_VARIABLES) {
			moderror_msg("Maximum number of parameters is %d. The module uses %d. "
					"Increase MAX_VARIABLES in swapi_static/skeleton/skeleton.c and recompile ALOE\n",
					MAX_VARIABLES,nof_vars);
			return -1;
		}
	}

	return 0;
}

int init_interfaces(void *ctx) {
	int i;

	for (i=0;i<nof_input_itf;i++) {
		moddebug("input %d\n",i);
		inputs[i] = swapi_itf_create(ctx, i, ITF_READ, input_max_samples*input_sample_sz);
		if (inputs[i] == NULL) {
			if (swapi_error_code(ctx) == SWAPI_ERROR_NOTREADY) {
				moddebug("input_trials=%d\n",input_trials);
				modinfo_msg("input port %d not yet configured. Trying again %d/%d...\n",i,
						input_trials,MAX_INPUT_TRIALS);
				input_trials++;
				if (input_trials == MAX_INPUT_TRIALS) {
					moderror_msg("input port %d not connected after %d trials\n",MAX_INPUT_TRIALS);
					return -1;
				}
				return 0;
			} else {
				swapi_perror("swapi_itf_create\n");
				return -1;
			}
		}
	}

	for (i=0;i<nof_output_itf;i++) {
		moddebug("output %d\n",i);
		outputs[i] = swapi_itf_create(ctx, i, ITF_WRITE, output_max_samples*output_sample_sz);
		if (outputs[i] == NULL) {
			swapi_perror("swapi_itf_create\n");
			return -1;
		}
		input_trials = 0;
	}

#ifdef _ALOE_OLD_SKELETON

	input_buffer = malloc(nof_input_itf*input_max_samples*input_sample_sz);
	if (!input_buffer) {
		perror("malloc");
		return -1;
	}
	output_buffer = malloc(nof_output_itf*output_max_samples*output_sample_sz);
	if (!output_buffer) {
		perror("malloc");
		return -1;
	}

#endif


	return 1;
}

void close_interfaces(void *ctx) {
	int i;
	moddebug("nof_input=%d, nof_output=%d\n",nof_input_itf,nof_output_itf);

	for (i=0;i<nof_input_itf;i++) {
		if (inputs[i]) {
			if (swapi_itf_close(inputs[i])) {
				swapi_perror("swapi_itf_close");
			}
		}
	}
	for (i=0;i<nof_output_itf;i++) {
		if (outputs[i]) {
			if (swapi_itf_close(outputs[i])) {
				swapi_perror("swapi_itf_close");
			}
		}
	}
}

int init_parameters(void *ctx) {
	int i;
	param_t *param;

	moddebug("nof_params=%d\n",nof_params);

	if (!nof_params || !user_params) {
		return 0;
	}

	if (param_init(user_params,nof_params) < 0) {
		moderror_msg("initializing %d params: %s\n",nof_params,param_error_str());
		return -1;
	}

	if (nof_params != param_nof()) {
		modinfo_msg("warning: user configured %d parameters but %d where read\n",nof_params,
				param_nof());
	}

	for (i=0;i<param_nof();i++) {
		moddebug("param %d\n",i);
		param = param_get_i(i);
		assert(param);
		if (swapi_var_initialize(ctx, param->name, param_get_addr_i(i), param_get_bsize_i(i)) == -1) {
			swapi_perror("swapi_var_initialize\n");
			moderror_msg("parameter name=%s size=%d\n",param->name, param_get_bsize_i(i));
			return -1;
		}
	}
#ifdef _ALOE_OLD_SKELETON
	if (input_buffer) {
		free(input_buffer);
	}
	if (output_buffer) {
		free(output_buffer);
	}
#endif
	return 0;
}

void close_parameters(void *ctx) {
	moddebug("param_nof=%d\n",param_nof());
}

int init_variables(void *ctx) {
	int i;

	moddebug("nof_vars=%d\n",nof_vars);

	for (i=0;nof_vars;i++) {
		moddebug("var %d\n",i);
		vars[i]=swapi_var_create(ctx, user_vars[i].name, user_vars[i].value, user_vars[i].size);
		if (!vars[i]) {
			swapi_perror("swapi_var_create\n");
			moderror_msg("variable name=%s size=%d\n",user_vars[i].name, user_vars[i].size);
			return -1;
		}
	}
	return 0;
}

void close_variables(void *ctx) {
	int i;
	moddebug("nof_vars=%d\n",nof_vars);
	for (i=0;i<nof_vars;i++) {
		if (vars[i]) {
			if (swapi_var_close(ctx, vars[i])) {
				swapi_perror("swapi_var_close\n");
			}
		}
	}
}

int init_log(void *ctx) {
	mlog = swapi_log_create(ctx, "default");
	moddebug("log=0x%x\n",mlog);
	if (!mlog) {
		swapi_perror("swapi_log_create\n");
		return -1;
	}
	return 0;
}

void close_log(void *ctx) {
	moddebug("log=0x%x\n",mlog);
	if (!mlog) return;
	if (swapi_log_close(mlog)) {
		swapi_perror("swapi_counter_close\n");
	}
}

int init_counter(void *ctx) {
	counter = swapi_counter_create(ctx, "work");
	moddebug("counter=0x%x\n",counter);
	if (!counter) {
		swapi_perror("swapi_counter_create\n");
		return -1;
	}
	return 0;
}

void close_counter(void *ctx) {
	moddebug("counter=0x%x\n",counter);
	if (!counter) return;
	if (swapi_counter_close(counter)) {
		swapi_perror("swapi_counter_close\n");
	}
}

int Init(void *ctx) {
	int n;
	context = ctx;

	moddebug("enter ts=%d\n",swapi_tstamp(ctx));

	init_memory();

	mlog = NULL;
	if (USE_LOG) {
		if (init_log(ctx)) {
			return -1;
		}
	}

	if (check_configuration(ctx)) {
		return -1;
	}
	n = init_interfaces(ctx);
	if (n == -1) {
		return -1;
	} else if (n == 0) {
		return 0;
	}

	if (init_parameters(ctx)) {
		return -1;
	}

	if (init_variables(ctx)) {
		return -1;
	}

	if (init_counter(ctx)) {
		return -1;
	}

	moddebug("calling initialize, ts=%d\n",swapi_tstamp(ctx));

	/* this is the module initialize function */
	initialize();

	moddebug("exit ts=%d\n",swapi_tstamp(ctx));
	return 1;
}

int Stop(void *ctx) {
	moddebug("enter ts=%d\n",swapi_tstamp(ctx));

	close_counter(ctx);
	close_variables(ctx);
	close_parameters(ctx);
	close_interfaces(ctx);

	moddebug("exit ts=%d\n",swapi_tstamp(ctx));
	swapi_exit(ctx);
	return 0;
}


int Run(void *ctx) {
	moddebug("enter ts=%d\n",swapi_tstamp(ctx));
	int i;
	int n;

	for (i=0;i<nof_input_itf;i++) {
		input_pkt[i] = swapi_itf_pkt_get(inputs[i]);
		moddebug("get: input=%d, pkt=0x%x\n",i, input_pkt[i]);
		if (!input_pkt[i]) {
			rcv_len[i] = 0;
		} else {
			moddebug("get: input=%d, len=%d\n",i,input_pkt[i]->len);
			rcv_len[i] = input_pkt[i]->len/input_sample_sz;
		}
	}
	for (i=0;i<nof_output_itf;i++) {
		output_pkt[i] = swapi_itf_pkt_request(outputs[i]);
		moddebug("request: output=%d pkt=0x%x\n",i,output_pkt[i]);
		if (!output_pkt[i]) {
			swapi_perror("swapi_itf_ptr_request\n");
			return -1;
		}
	}

	memset(snd_len,0,sizeof(int)*nof_input_itf);

	/* for legacy aloe modules */
#ifdef _ALOE_OLD_SKELETON

	for (i=0;i<nof_input_itf;i++) {
		if (input_pkt[i]) {
			assert(input_pkt[i]->data);
			memcpy(&input_buffer[i*input_max_samples*input_sample_sz],input_pkt[i]->data,
					rcv_len[i]*input_sample_sz);
		}
	}

	/* This is the module DSP function */
	moddebug("work ts=%d\n",swapi_tstamp(context));
	n = work(input_buffer,output_buffer);
	if (n<0) {
		return -1;
	}

#else
	/* This is the module DSP function */
	for (i=0;i<nof_input_itf;i++) {
		if (input_pkt[i]) {
			input_ptr[i]=input_pkt[i]->data;
		}
	}
	for (i=0;i<nof_output_itf;i++) {
		if (output_pkt[i]) {
			output_ptr[i]=output_pkt[i]->data;
		}
	}
	n = work(input_ptr,output_ptr);
	if (n<0) {
		return -1;
	}

#endif


	memset(rcv_len,0,sizeof(int)*nof_input_itf);

	for (i=0;i<nof_output_itf;i++) {
		if (!snd_len[i]) {
			snd_len[i] = n;
		}
	}

#ifdef _ALOE_OLD_SKELETON
	for (i=0;i<nof_output_itf;i++) {
		if (output_pkt[i]) {
			assert(output_pkt[i]->data);
			memcpy(output_pkt[i]->data,&output_buffer[i*output_max_samples*output_sample_sz],
					snd_len[i]*output_sample_sz);
		}
	}

#endif

	for (i=0;i<nof_input_itf;i++) {
		moddebug("release: input=%d pkt=0x%x\n",i,input_pkt[i]);
		if (input_pkt[i]) {
			n = swapi_itf_pkt_release(inputs[i],input_pkt[i]);
			if (n == -1) {
				swapi_perror("swapi_itf_ptr_release\n");
				return -1;
			} else if (n == 0) {
				modinfo_msg("warning packet from input interface %d could not be released\n",i);
			}
		}
	}
	for (i=0;i<nof_output_itf;i++) {
		if (output_pkt[i]) {
			moddebug("put: output=%d pkt=0x%x len=%d\n",i,output_pkt[i],snd_len[i]);
			if (snd_len[i]) {
				output_pkt[i]->len = snd_len[i];
				n = swapi_itf_pkt_put(outputs[i],output_pkt[i]);
				if (n == -1) {
					swapi_perror("swapi_itf_ptr_put\n");
					return -1;
				} else if (n == 0) {
					moderror_msg("warning packet from output interface %d could not be transmitted\n",i);
				}
			} else {
				n = swapi_itf_pkt_release(outputs[i],output_pkt[i]);
				if (n == -1) {
					swapi_perror("swapi_itf_ptr_release\n");
					return -1;
				} else if (n == 0) {
					modinfo_msg("warning packet from output interface %d could not be released\n",i);
				}
			}
		}
	}

	moddebug("exit ts=%d\n",swapi_tstamp(ctx));
	return 0;
}


int get_input_samples(int idx) {
	assert(idx>=0 && idx<nof_input_itf);
	return rcv_len[idx];
}

void set_output_samples(int idx, int len) {
	assert(idx>=0 && idx<nof_output_itf);
	assert(len>=0);
	snd_len[idx] = len;
}

int get_input_max_samples() {
	return input_max_samples;
}

int get_output_max_samples() {
	return output_max_samples;
}

