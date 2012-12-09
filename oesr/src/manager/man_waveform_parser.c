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
#include <libconfig.h>

#include "str.h"
#include "defs.h"
#include "waveform.h"
#include "objects_max.h"
#include "mempool.h"

config_t config;

#define ITF_PREALLOC 30

static int mod_id=1, waveform_id=1;

/**@TODO: Generate waveform model
 *
 */
/*static void generate_model_from_waveform(waveform_t *w) {
	aerror("Not yet implemented\n");
}
*/

static int find_mode(waveform_t *w, const char *name) {
	int i;
	for (i=0;i<w->nof_modes;i++) {
		if (!strncmp(w->modes[i].name,name,STR_LEN)) {
			break;
		}
	}
	if (i==w->nof_modes)
		return -1;
	else
		return i;
}

#define check_and_set_size(a) if (v->size) { if (v->size != a) {\
	aerror_msg("previous variable size was %d but mode %d is %d\n",v->size,idx,a); return 0;}\
	} else { v->size = a;}


static int read_variable_value(config_setting_t *value, variable_t *v, int idx) {
	const char *tmp;
	int i,n;
	config_setting_t *val;
	int *iptr;
	float *fptr;

	switch(config_setting_type(value)) {
	case CONFIG_TYPE_INT:
		check_and_set_size(sizeof(int));
		v->init_value[idx] = malloc(sizeof(int));
		assert(v->init_value[idx]);
		*((int*) v->init_value[idx]) = config_setting_get_int(value);
		v->type = VAR_TYPE_INT;
		pardebug("%s: mode=%d, int=%d\n",v->name,idx,*((int*) v->init_value[idx]));
		break;
	case CONFIG_TYPE_FLOAT:
		check_and_set_size(sizeof(float));
		v->init_value[idx] = malloc(sizeof(float));
		assert(v->init_value[idx]);
		v->type = VAR_TYPE_FLOAT;
		*((float*) v->init_value[idx]) = (float) config_setting_get_float(value);
		pardebug("%s: mode=%d, float=%.2f\n",v->name,idx,*((float*) v->init_value[idx]));
		break;
	case CONFIG_TYPE_STRING:
		tmp = config_setting_get_string(value);
		if (!tmp) return 0;
		check_and_set_size(STR_LEN);
		v->init_value[idx] = malloc(STR_LEN+1);
		assert(v->init_value[idx]);
		v->type = VAR_TYPE_STRING;
		strncpy(v->init_value[idx],tmp,STR_LEN);
		pardebug("%s: mode=%d, string=%s\n",v->name,idx,v->init_value[idx]);
		break;
	case CONFIG_TYPE_ARRAY:
		val = config_setting_get_elem(value,0);
		n = config_setting_length(value);
		switch(config_setting_type(val)) {
		case CONFIG_TYPE_INT:
			check_and_set_size(n*(int) sizeof(int));
			v->init_value[idx] = malloc((size_t) v->size);
			iptr=v->init_value[idx];
			v->type = VAR_TYPE_INT;
			for (i=0;i<n;i++) {
				iptr[i] = config_setting_get_int_elem(value,i);
				pardebug("%s: mode=%d, int[%d]=%.2f\n",v->name,idx,i,
						iptr[i]);
			}
			break;
		case CONFIG_TYPE_FLOAT:
			check_and_set_size(n*(int) sizeof(float));
			v->init_value[idx] = malloc((size_t) v->size);
			fptr=v->init_value[idx];
			v->type = VAR_TYPE_FLOAT;
			for (i=0;i<n;i++) {
				fptr[i] = (float) config_setting_get_float_elem(value, i);
				pardebug("%s: mode=%d, float[%d]=%.2f\n",v->name,idx,i,
						fptr[i]);
			}
			break;
		default:
			aerror("Unsupported type\n");
			return 0;
		}

		break;
	default:
		aerror("Unsupported type\n");
		return 0;
	}
	return 1;
}

static int read_variable(config_setting_t *cfg, variable_t *var, waveform_t *w) {
	const char *tmp;
	int i, m, nof_modes;
	config_setting_t *value, *modevalue, *modevalue2;
	if (!config_setting_lookup_string(cfg, "name", &tmp)) {
		aerror("missing name\n");
		return 0;
	}
	strcpy(var->name,tmp);
	value = config_setting_get_member(cfg, "value");
	if (!value) {
		aerror("missing value\n");
		return 0;
	}
	nof_modes = config_setting_length(value);
	pardebug("variable %s has %d modes\n",var->name,nof_modes);
	if (nof_modes == 0 || nof_modes == 1) {
		if (nof_modes == 1) {
			modevalue2 = config_setting_get_elem(value,0);
		} else {
			modevalue2 = value;
		}
		for (i=0;i<w->nof_modes;i++) {
			if (!read_variable_value(modevalue2, var, i)) {
				aerror("parsing value\n");
				return 0;
			}
		}
	} else {
		if (nof_modes != w->nof_modes) {
			aerror_msg("%d modes defined in mopts but expected %d\n",
					nof_modes,w->nof_modes);
			return 0;
		}
		for (i=0;i<nof_modes;i++) {
			modevalue = config_setting_get_elem(value, (unsigned int) i);
			tmp = config_setting_get_string_elem(modevalue,0);
			if (!tmp) {
				aerror_msg("missing mode name for mode %d\n",i);
				return 0;
			}
			m = find_mode(w,tmp);
			if (m == -1) {
				aerror_msg("can't find mode %s\n", tmp);
				return 0;
			}
			modevalue2 = config_setting_get_elem(modevalue,1);
			if (!modevalue2) {
				aerror_msg("missing value for mode %d\n",i);
				return 0;
			}
			if (!read_variable_value(modevalue2, var, m)) {
				aerror("parsing value\n");
				return 0;
			}
		}
	}
	return 1;
}

static int read_variables(config_setting_t *cfg, module_t *mod, char *module_name, waveform_t *w) {
	int nof_variables, i;
	config_setting_t *varscfg, *var;
	varscfg = config_setting_get_member(cfg, "variables");
	if (!varscfg) {
		return 0;
	}

	nof_variables = config_setting_length(varscfg);
	mod->variables = (variable_t*) pool_alloc(nof_variables,sizeof(variable_t));
	if (!mod->variables) return -1;
	pardebug("found %d variables module_id=%d\n",nof_variables,mod->id);
	for (i=0;i<nof_variables;i++) {
		pardebug("reading variable %d\n",i);
		var = config_setting_get_elem(varscfg, (unsigned int) i);
		assert(var);
		if (!read_variable(var, &mod->variables[i],w)) {
			aerror_msg("reading variable %d for module %s\n",i,module_name);
			return 0;
		}
		mod->variables[i].id=i+1;
	}

	return nof_variables;
}

static int read_interface_pair(config_setting_t *cfg, const char **name, int *idx) {
	if (config_setting_length(cfg) == 0) {
		*name = config_setting_get_string(cfg);
		if (!*name) {
			return 0;
		}
		*idx = 0;
		return 1;
	} else if (config_setting_length(cfg) <= 2) {
		*name = config_setting_get_string_elem(cfg,0);
		if (!*name) {
			return 0;
		}
		if (config_setting_length(cfg) == 2) {
			*idx = config_setting_get_int_elem(cfg,1);
		} else {
			*idx = 0;
		}
		return 1;
	} else {
		return 0;
	}
}

static int read_interface_mod_itf(config_setting_t *cfg, waveform_t *w,
		module_t **mod, int *port) {
	const char *ctmp;
	if (!read_interface_pair(cfg,&ctmp,port)) {
		return 0;
	}
	*mod = waveform_find_module_name(w,(char*) ctmp);
	if (!*mod) {
		aerror_msg("destination module %s not found\n", ctmp);
		return 0;
	}
	if (*port<0) {
		aerror_msg("invalid port %d\n",*port);
		return 0;
	}
	return 1;
}

static int read_interface(config_setting_t *cfg, waveform_t *w) {
	config_setting_t *src, *dest;
	module_t *src_module, *dest_module;
	interface_t *src_itf, *dest_itf;
	int src_port, dest_port;
	double tmp;

	src = config_setting_get_member(cfg, "src");
	if (!src) {
		aerror("missing src\n");
		return 0;
	}
	dest = config_setting_get_member(cfg, "dest");
	if (!dest) {
		aerror("missing dest\n");
		return 0;
	}
	if (!read_interface_mod_itf(src,w,&src_module,&src_port)) {
		aerror("reading src\n");
		return 0;
	}

	if (src_port > ITF_PREALLOC) {
		aerror_msg("src port too high. Maximum is %d\n",ITF_PREALLOC);
		return 0;
	}

	src_itf = &src_module->outputs[src_port];
	if (!read_interface_mod_itf(dest,w,&dest_module,&dest_port)) {
		aerror("reading src\n");
		return 0;
	}
	if (dest_port > ITF_PREALLOC) {
		aerror_msg("src port too high. Maximum is %d\n",ITF_PREALLOC);
		return 0;
	}
	dest_itf = &dest_module->inputs[dest_port];

	src_itf->remote_module_id = dest_module->id;
	src_itf->remote_port_idx = dest_port;

	if (dest_itf->remote_module_id) {
		aerror_msg("port %d in module %s already connected (source %s:%d)\n",
				dest_port,dest_module->name,
				src->name,src_port);
		return 0;
	}

	dest_itf->remote_module_id = src_module->id;
	dest_itf->remote_port_idx = src_port;

	if (config_setting_lookup_float(cfg, "mbpts", &tmp)) {
		src_itf->total_mbpts = (float) tmp;
	} else {
		src_itf->total_mbpts = 1;
	}
	dest_itf->total_mbpts = src_itf->total_mbpts;

	if (!config_setting_lookup_int(cfg, "delay", &src_itf->delay)) {
		src_itf->delay = 0;
	}
	dest_module->nof_inputs++;
	src_module->nof_outputs++;
	pardebug("%d:%d/%d->%d:%d/%d bw=%.2f delay=%d\n",src_module->id,src_port,src_module->nof_outputs,
			dest_module->id,dest_port,dest_module->nof_outputs, src_itf->total_mbpts,
			src_itf->delay);
	return 1;
}

static int read_mopts_mode(config_setting_t *cfg, module_t *mod, waveform_t *w) {
	const char *tmp;
	int i;

	tmp = config_setting_get_string_elem(cfg,0);
	if (!tmp) {
		aerror("reading mode name\n");
		return 0;
	}
	i = find_mode(w,tmp);
	if (i==-1) {
		aerror_msg("mode %s not found\n",tmp);
		return 0;
	}
	mod->c_mopts[i] = (float) config_setting_get_float_elem(cfg,1);
	pardebug("[array] mopts[%d]=%.2f\n",i,mod->c_mopts[i]);
	return 1;
}

static int read_module(config_setting_t *cfg, module_t *mod, waveform_t *w) {
	const char *tmp;
	int n_vars,i;
	int imopts;
	config_setting_t *mopts, *mopts_mode;
	tmp = config_setting_name(cfg);
	if (!tmp) {
		aerror("Missing module name\n");
		return 0;
	}
	strcpy(mod->name, tmp);
	if (!config_setting_lookup_string(cfg, "binary", &tmp)) {
		aerror_msg("binary field not found in module %s\n",mod->name);
		return 0;
	}
	strcpy(mod->binary, tmp);
	mopts = config_setting_get_member(cfg, "mopts");
	if (config_setting_type(mopts) == CONFIG_TYPE_INT) {
		for (i=0;i<w->nof_modes;i++) {
			mod->c_mopts[i] = (float) config_setting_get_int(mopts);
			pardebug("[int] mopts[%d]=%.2f\n",i,mod->c_mopts[i]);
		}
	} else if (config_setting_type(mopts) == CONFIG_TYPE_FLOAT) {
		for (i=0;i<w->nof_modes;i++) {
			mod->c_mopts[i] = (float) config_setting_get_float(mopts);
			pardebug("[float] mopts[%d]=%.2f\n",i,mod->c_mopts[i]);
		}
	} else {
		imopts = config_setting_length(mopts);
		if (imopts != w->nof_modes) {
			aerror_msg("%d modes defined in mopts but expected %d\n",imopts,w->nof_modes);
			return 0;
		}
		for (i=0;i<imopts;i++) {
			mopts_mode = config_setting_get_elem(mopts, (unsigned int) i);
			assert(mopts_mode);
			if (!read_mopts_mode(mopts_mode,mod,w)) {
				aerror_msg("reading mopts mode %d\n",i);
				return 0;
			}
		}
	}
	n_vars = read_variables(cfg, mod, mod->name, w);
	if (n_vars < 0) {
		aerror_msg("reading variables for module %s\n", mod->name);
		return 0;
	}

	mod->inputs = (interface_t*) pool_alloc(ITF_PREALLOC,sizeof(interface_t));
	if (!mod->inputs) {
		aerror("allocating memory\n");
		return 0;
	}
	mod->outputs = (interface_t*) pool_alloc(ITF_PREALLOC,sizeof(interface_t));
	if (!mod->outputs) {
		aerror("allocating memory\n");
		return 0;
	}
	mod->nof_variables = n_vars;
	mod->id = mod_id++;
	pardebug("parsed id=%d has %d variables\n",mod->id, mod->nof_variables);
	return 1;
}

int read_mode(config_setting_t *cfg, waveform_mode_t *m) {
	const char *tmp;
	if (!config_setting_lookup_string(cfg, "name", &tmp)) {
		aerror("name field not found\n");
		return 0;
	}
	strcpy(m->name,tmp);
	pardebug("read mode %s\n",m->name);
	if (!config_setting_lookup_string(cfg, "desc", &tmp)) {
		aerror("desc field not found\n");
		return 0;
	}
	strcpy(m->desc,tmp);
	return 1;
}

void check_interfaces(module_t *module) {
	pardebug("%d has %d inputs and %d outputs\n",module->id, module->nof_inputs,
				module->nof_outputs);
	int i;
	for (i=0;i<module->nof_inputs;i++) {
		module->inputs[i].id = i;
		if (!module->inputs[i].remote_module_id) {
			aerror_msg("warning: module %s has %d input ports but port %d "
					"has not been connected\n",module->name,module->nof_inputs,
					i);
		}
	}
	for (i=0;i<module->nof_outputs;i++) {
		module->outputs[i].id = module->nof_inputs+i;
		if (!module->outputs[i].remote_module_id) {
			aerror_msg("warning: module %s has %d output ports but port %d "
					"has not been connected\n",module->name,module->nof_outputs,
					i);
		}
	}
}

int realloc_interfaces(module_t *module) {
	pardebug("%d has %d inputs and %d outputs\n",module->id, module->nof_inputs,
			module->nof_outputs);
	int i;
	for (i=0;i<module->nof_inputs;i++) {
		module->inputs = (interface_t*) pool_realloc(module->inputs,module->nof_inputs,
				sizeof(interface_t));
		if (!module->inputs) {
			aerror("error with realloc\n");
			return 0;
		}
	}
	for (i=0;i<module->nof_outputs;i++) {
		module->outputs = (interface_t*) pool_realloc(module->outputs,module->nof_outputs,
				sizeof(interface_t));
		if (!module->outputs) {
			aerror("error with realloc\n");
			return 0;
		}
	}
	return 1;
}

/** \brief Fills the contents of the object w with the configuration of file w->model_file.
 * waveform_parse() allocates memory for modules, interfaces and variables as required by the model file.
 * It also generates the waveform resource model (c,b) and saves it in w->c and w->b.
 * This function uses LGPL libconfig library (http://www.hyperrealm.com/libconfig/).
 * \returns 0 on success or -1 on error, using the oesr_man_error class for error description.
 */
int waveform_parse(waveform_t* w) {
	/** @TODO: Use oesr_man error interface */
	aassert(w);
	int ret;
	config_setting_t *modules, *modcfg, *interfaces, *itfcfg, *modes, *modecfg;
	int nof_modules, nof_itfs,nof_modes;
	int i;
	module_t *mod;
	ret = -1;

	config_init(&config);
	pardebug("waveform_name=%s, model_file %s\n",w->name,w->model_file);
	if (!config_read_file(&config, w->model_file)) {
		aerror_msg("line %d - %s: \n", config_error_line(&config),
				config_error_text(&config));
		goto destroy;
	}
	modes = config_lookup(&config, "modes");
	if (!modes) {
		pardebug("found %d mode\n",1);
		w->nof_modes = 1;
		strcpy(w->modes[0].name, "default");
		strcpy(w->modes[0].desc, "default");
	} else {
		nof_modes = config_setting_length(modes);
		if (!nof_modes) {
			aerror("Any mode was found\n");
			goto destroy;
		}
		pardebug("found %d modes\n",nof_modes);
		if (nof_modes > MAX(modes)) {
			aerror_msg("Error maximum number of modes is %d\n",MAX(modes));
			goto destroy;
		}
		w->nof_modes = nof_modes;
		for (i=0;i<nof_modes;i++) {
			pardebug("configuring mode %d\n",i);
			modecfg = config_setting_get_elem(modes, (unsigned int) i);
			assert(modecfg);
			if (!read_mode(modecfg,&w->modes[i])) {
				aerror_msg("reading mode %d\n", i);
				goto destroy;
			}
		}
	}

	modules = config_lookup(&config, "modules");
	if (!modules) {
		aerror("Section modules not found\n");
		goto destroy;
	}
	nof_modules = config_setting_length(modules);
	pardebug("found %d modules\n",nof_modules);
	if (!nof_modules) {
		aerror("Any module was found\n");
		goto destroy;
	}
	if (waveform_alloc(w,nof_modules)) {
		aerror("allocating modules\n");
		goto destroy;
	}
	/* first read all modules */
	for (i=0;i<nof_modules;i++) {
		pardebug("parsing module %d\n",i);
		mod = &w->modules[i];
		modcfg = config_setting_get_elem(modules, (unsigned int) i);
		assert(modcfg);
		if (!read_module(modcfg,mod, w)) {
			aerror_msg("parsing module %d\n",i);
			goto destroy;
		}
		w->modules[i].waveform = w;
		w->modules[i].nof_modes = w->nof_modes;
	}
	interfaces = config_lookup(&config, "interfaces");
	if (!interfaces) {
		aerror("Section interfaces not found\n");
		goto destroy;
	}
	nof_itfs=config_setting_length(interfaces);
	pardebug("found %d interfaces\n",nof_itfs);
	if (!nof_itfs) {
		aerror("Any module was found\n");
		goto destroy;
	}
	/* now parse interfaces */
	for (i=0;i<nof_itfs;i++) {
		pardebug("parsing itf %d\n",i);
		itfcfg = config_setting_get_elem(interfaces, (unsigned int) i);
		assert(itfcfg);
		if (!read_interface(itfcfg,w)) {
			aerror_msg("parsing interface %d\n", i);
			goto destroy;
		}
	}
	for (i=0;i<w->nof_modules;i++) {
		pardebug("checking and reallocing module %d\n",i);
		check_interfaces(&w->modules[i]);
		if (!realloc_interfaces(&w->modules[i])) {
			goto destroy;
		}
	}
	w->id = waveform_id++;
	pardebug("done, waveform_id=%d\n",w->id);
	ret = 0;
destroy:
	config_destroy(&config);
	return ret;
}
