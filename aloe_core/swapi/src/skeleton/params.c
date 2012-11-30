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


/** PARAMETERS MANAGER */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _COMPILE_ALOE
#include <phal_sw_api.h>
#endif

#include "params.h"

#define ERROR_MSG_LEN 1024

struct param_memory {
	int *int_values;
	float *float_values;
	_Complex float *cplx_values;
	char *strings;
};

struct param_fields {
	param_t field;
	void *mem;
};

static void *packet;
static int packet_sz;

static int nof_fields=0;
static struct param_memory mem;
static struct param_fields ctrl[MAX_PARAMS];

static int initialized=0;

static char error_msg[ERROR_MSG_LEN];
#define param_error(_fmt, ...) snprintf(error_msg,ERROR_MSG_LEN,_fmt,__VA_ARGS__)
#define assert_initialized	do { if(!initialized) { \
	param_error("[line %d]: Must call params_init() before calling any function\n"\
			,__LINE__); return -1; } } while(0)
#define assert_initialized_p	do { if(!initialized) { \
	param_error("[line %d]: Must call params_init() before calling any function\n"\
			,__LINE__); return NULL; } } while(0)
#define assert_param(a) do {if (!(a)) { param_error("[line %d]: Parameter assertion failed (%s)\n",\
		__LINE__,__STRING(a)); return -1;} } while(0)
#define assert_param_p(a) do {if (!(a)) { param_error("[line %d]: Parameter assertion failed (%s)\n",\
		__LINE__,__STRING(a)); return NULL;} } while(0)

#define get_pos(pos,name) do {pos=param_get_idx(name); if (pos==-1) { \
	param_error("[line %d]: Parameter name %s not found\n",\
		__LINE__,name); return -1;} } while(0)
#define get_pos_p(pos,name) do {pos=param_get_idx(name); if (pos==-1) { \
	param_error("[line %d]: Parameter name %s not found\n",\
		__LINE__,name); return NULL;} } while(0)


#define assert_idx assert_param(idx>=0); assert_param(idx<nof_fields);
#define assert_idx_p assert_param_p(idx>=0); assert_param_p(idx<nof_fields);




/**
 * @brief Reads up to buff_sz param_t objects from the user_params vector.
 * Allocates memory for their values and initialized the internal structures.
 *
 * @param user_params Pointer to the first param_t object
 * @param buff_size Number of param_t objects in user_param buffer
 *
 * @returns On success, returns a non-negative integer indicating the number of parameters that have
 * been parsed. On error returns -1.
 */
int param_init(param_t *user_params, int buff_sz) {
	int i;
	int c_int=0, c_float=0, c_cplx=0, c_str=0;
	assert_param(user_params);
	assert_param(buff_sz);

	if (initialized) {
		param_error("Already initialized (%d)\n",initialized);
		return -1;
	}

	for (nof_fields=0;user_params[nof_fields].name && nof_fields<MAX_PARAMS
			&& nof_fields<buff_sz; nof_fields++) {
		switch(user_params[nof_fields].type) {
		case INT:
			c_int+=user_params[nof_fields].size;
			break;
		case FLOAT:
			c_float+=user_params[nof_fields].size;
			break;
		case COMPLEX:
			c_cplx+=user_params[nof_fields].size;
			break;
		case STRING:
			c_str++;
			break;
		default:
			param_error("Unknown parameter %d\n", (int) user_params[nof_fields].type);
			return -1;
		}
	}

	if (!nof_fields) {
		return 0;
	}

	packet_sz = c_int*sizeof(int)+c_cplx*sizeof(_Complex float)+c_float*sizeof(float)+
			c_str*STR_LEN*sizeof(char);
	packet = calloc(1,packet_sz);

	mem.int_values = calloc(1,sizeof(int)*c_int);
	mem.cplx_values = calloc(1,sizeof(_Complex float)*c_cplx);
	mem.float_values = calloc(1,sizeof(float)*c_float);
	mem.strings = calloc(1,sizeof(char)*STR_LEN*c_str);

	c_int=c_cplx=c_float=c_str=0;
	for (i=0;i<nof_fields;i++) {
		memcpy(&ctrl[i].field,&user_params[i],sizeof(param_t));
		switch(user_params[i].type) {
		case INT:
			ctrl[i].mem = &mem.int_values[c_int];
			c_int+=ctrl[i].field.size;
			break;
		case FLOAT:
			ctrl[i].mem = &mem.float_values[c_float];
			c_float+=ctrl[i].field.size;;
			break;
		case COMPLEX:
			ctrl[i].mem = &mem.cplx_values[c_cplx];
			c_cplx+=ctrl[i].field.size;;
			break;
		case STRING:
			ctrl[i].mem = &mem.cplx_values[c_cplx];
			c_str+=STR_LEN;
			break;
		default:
			param_error("Unknown parameter %d\n", (int) user_params[nof_fields].type);
			return -1;
		}
	}
	initialized=1;
	return nof_fields;
}

int param_close() {
	assert_initialized;

	free(packet);
	free(mem.int_values);
	free(mem.cplx_values);
	free(mem.float_values);
	free(mem.strings);

	return -1;
}

/**
 * @brief Returns a pointer to a string describing the last error produced during
 * a call to the param api.
 * @returns pointer to a string
 */
char* param_error_str() {
	return error_msg;
}


/**
 * @brief Returns the total size in memory required to store all the
 * parameters values. Useful to send/receive packets through control interfaces.
 *
 * @returns The packet size in bytes or -1 on error
 */
int param_packet_size() {
	assert_initialized;
	return packet_sz;
}

/**
 * @brief Returns the address of a intermediate buffer allocated during
 * param_init() where packets with parameter values may be stored. Useful to
 * send/receive packets through control interfaces.
 *
 * @returns On success the function returns a non-null pointer to the buffer address.
 * On error returns a NULL pointer.
 */
void* param_packet_addr() {
	assert_initialized_p;
	return packet;
}

/**
 * @brief Reads the data from an intermediate buffer pointed by param_packet_addr()
 * where parameter values are saved and copies the contents to the internal
 * memory for user utilization. Useful to receive packets from the control
 * interface.
 *
 * @param rcv_len Number of bytes to read from the intermediate buffer (i.e. received
 * control packet length in bytes).
 *
 * @returns A non-negative number indicating the number of bytes that where successfully
 * read from the packet or -1 on error.
 */
int param_packet_read(int rcv_len) {
	param_error("Not initialized (%d)\n",initialized);
	return -1;
}

/**
 * @brief Writes the contents of the parameter values to the intermediate buffer
 * pointed by param_packet_addr(). Useful to send packets to the control
 * interface.
 *
 * @returns A non-negative integer indicating the number of bytes that where stored
 * in the buffer on success or -1 on error.
 */
int param_packet_write() {
	param_error("Not initialized (%d)\n",initialized);
	return -1;
}

int param_nof() {
	return nof_fields;
}

/**\param returns the size of the parameter in bytes */
int param_get_bsize(char *name) {
	assert_initialized;
	assert_param(name);
	int pos;
	get_pos(pos,name);
	return param_get_bsize_i(pos);
}

/**\param returns the size of the parameter in bytes */
int param_get_bsize_i(int idx) {
	assert_initialized;
	assert_idx;
	switch(ctrl[idx].field.type) {
	case INT:
		return sizeof(int)*ctrl[idx].field.size;
		break;
	case FLOAT:
		return sizeof(float)*ctrl[idx].field.size;
		break;
	case COMPLEX:
		return sizeof(_Complex float)*ctrl[idx].field.size;
		break;
	case STRING:
		return STR_LEN;
		break;
	default:
		param_error("Unknown parameter %d\n", ctrl[idx].field.type);
		return -1;
	}
}

/**
 * @brief param_get_idx() returns a non-negative integer associated to the parameter
 * with name equal to the first argument. This integer may be used in subsequent
 * calls to param_get_i(), param_get_addr_i() and param_get_aloe_type_i().
 * @param name Name of the parameter
 * @return non-negative integer on success, -1 on error
 */
int	param_get_idx(char *name) {
	assert_initialized;
	assert_param(name);

	int i=0;
	while(i<nof_fields && strcmp(ctrl[i].field.name,name))
		i++;
	if (i==nof_fields) {
		return -1;
	}

	return i;
}

/** @brief Returns a pointer to the param_t structure of the parameter with name name.
 * @param name Name of the parameter
 * @return non-null pointer on success or null on error
 */
param_t* param_get(char *name) {
	int pos;
	assert_param_p(name);
	get_pos_p(pos,name);
	return &ctrl[pos].field;
}
/** @brief Returns a pointer to the param_t structure of the parameter with index idx
 * returned by the param_get_idx() function.
 * @param idx non-negative integer idx value returned by param_get_idx
 * @return non-null pointer on success or null on error
 */
param_t* param_get_i(int idx) {
	assert_idx_p;
	return &ctrl[idx].field;
}

/** Returns the memory address allocated for the value of the parameter name
 * @param name Name of the parameter
 * @return non-null pointer on success or null on error
 */
void* param_get_addr(char *name) {
	int pos;
	assert_param_p(name);
	get_pos_p(pos,name);
	return ctrl[pos].mem;
}
void* param_get_addr_i(int idx) {
	assert_idx_p
	return ctrl[idx].mem;
}

/** Returns the memory address allocated for the value of the parameter associated
 * with the idx returned by param_get_idx() function.
 * @param idx non-negative integer idx value returned by param_get_idx
 * @return non-null pointer on success or null on error
 */
void* param_get_ptr_i(int idx) {
	assert_idx_p;
	return ctrl[idx].mem;
}


int param_get_aloe_type(char *name) {
	int pos;
	assert_param(name);
	get_pos(pos,name);

#ifdef STAT_TYPE_INT

	switch(ctrl[pos].field.type) {
	case INT:
		return STAT_TYPE_INT;
	case FLOAT:
		return STAT_TYPE_FLOAT;
	case COMPLEX:
		printf("Warning complex parameters not yet supported\n");
		return STAT_TYPE_FLOAT;
	case STRING:
		return STAT_TYPE_CHAR;
	}
#else
	param_error("Must compile with ALOE stats.h (%d)\n",initialized);
	return -1;
#endif

}

int param_get_aloe_type_i(int idx) {
	assert_idx;

#ifdef STAT_TYPE_INT
	switch(ctrl[idx].field.type) {
	case INT:
		return STAT_TYPE_INT;
	case FLOAT:
		return STAT_TYPE_FLOAT;
	case COMPLEX:
		printf("Warning complex parameters not yet supported\n");
		return STAT_TYPE_FLOAT;
	case STRING:
		return STAT_TYPE_CHAR;
	}
#else
	param_error("Must compile with ALOE stats.h (%d)\n",initialized);
	return -1;
#endif

}


