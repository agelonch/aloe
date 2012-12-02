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
#include <string.h>
#include "params.h"

/**\brief Stores in the address of the second parameter the scalar integer value of the
 * parameter name.
 * Tries to parse the parameter as an integer. Read param_addr() for supported formats.
 * \returns 1 on success. If if the parameter format is not integer returns 0. If the
 * parameter is not found or there is any other error returns -1.
 */
int param_get_int_name(char *name, int *value) {
	pmid_t id = param_id(name);
	if (id == NULL) {
		return -1;
	}
	return (param_get_int(id,value)==-1)?-1:0;
}

/**\brief Stores in the address of the second parameter the scalar float value of the
 * parameter name.
 * Tries to parse the parameter as an float. Read param_addr() for supported formats.
 * \returns 1 on success. If if the parameter format is not float returns -1. If the
 * parameter is not found returns 0.
 */
int param_get_float_name(char *name, float *value) {
	pmid_t id = param_id(name);
	if (id == NULL) {
		return -1;
	}
	return (param_get_float(id,value)==-1)?-1:0;
}

/**\brief see param_id() */
int param_get_int(pmid_t id, int *value) {
	param_type_t type;
	int size;
	int max_size = sizeof(int);

	if ((size = param_get(id,value,max_size,&type)) == -1) {
		return -1;
	}
	if (size != max_size) {
		return 0;
	}
	if (type != INT) {
		return 0;
	}
	return 1;
}

/**\brief see param_id() */
int param_get_float(pmid_t id, float *value) {
	param_type_t type;
	int size;
	int max_size = sizeof(float);

	if ((size = param_get(id,value,max_size,&type)) == -1) {
		return -1;
	}
	if (size != max_size) {
		return 0;
	}
	if (type != FLOAT) {
		return 0;
	}
	return 1;
}

#ifdef _ALOE_OLD_SKELETON
#define MAX_PARAMS 50
typedef struct {
	int i_val;
	float f_val;
	char name[64];
	pmid_t id;
	param_type_t type;
}legparam_t;
legparam_t legparams[MAX_PARAMS];

void *param_get_addr(char *name) {
	char tmp[16];
	int size;
	void *ret;
	int i;
	param_type_t type;

	for (i=0;i<MAX_PARAMS;i++) {
		if (legparams[i].id) {
			if (!strcmp(legparams[i].name,name)) {
				break;
			}
		}
	}
	if (i<MAX_PARAMS) {
		if (type == INT) {
			return &legparams[i].i_val;
		} else if (type == FLOAT) {
			return &legparams[i].f_val;
		}
	}

	pmid_t id = param_id(name);
	if (id == NULL) {
		return NULL;
	}
	for(i=0;i<MAX_PARAMS;i++) {
		if (!legparams[i].id)
			break;
	}
	if (i==MAX_PARAMS) {
		return NULL;
	}
	size = param_get(id,tmp,16,&type);
	if (size == -1) {
		return NULL;
	}
	if (type == INT) {
		memcpy(&legparams[i].i_val,tmp,sizeof(int));
		ret = &legparams[i].i_val;
	} else if (type == FLOAT) {
		memcpy(&legparams[i].f_val,tmp,sizeof(float));
		ret = &legparams[i].f_val;
	}
	strncpy(legparams[i].name,name,64);
	legparams[i].id = id;
	legparams[i].type = type;
	return ret;
}

#endif

