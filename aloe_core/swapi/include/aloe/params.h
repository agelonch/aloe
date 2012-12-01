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

#define MAX_PARAMS 	100
#define STR_LEN		128

/** Parameter type */
typedef enum {INT, FLOAT, COMPLEX, STRING} param_type_t;

/** Parameter object */
typedef struct {
	char *name;
	param_type_t type;
	int size;
}param_t;

#ifdef _ALOE_OLD_SKELETON
enum stat_update {OFF, READ, WRITE};
struct utils_variables {
	char *name;
	int type;
	int size;
	void *value;
	enum stat_update update_mode;
};
#else
struct utils_variables {
	char *name;
	int size;
	void *value;
};
#endif

typedef struct utils_variables user_var_t;

/**\brief	This function must be implemented by the DSP module. It return a pointer to a buffer
 * of nof_variables structures of type variable_t. The function stores in nof_variables pointer
 * the number of variables to configure. Returns null if there are no variables to configure.
 */
user_var_t*		variables_list(int *nof_variables);

/**\brief	This function must be implemented by the DSP module. It return a pointer to a buffer
 * of nof_parameters structures of type param_t. The function stores in nof_paramters pointer
 * the number of parameters to configure. Returns null if there are no parameters to configure.
 */
param_t*		param_list(int *nof_parameters);

int 			param_init(param_t *user_params, int buff_sz);
int				param_close();
char*			param_error_str();

int 			param_packet_size();
void*			param_packet_addr();
int 			param_packet_read(int rcv_len);
int 			param_packet_write();

int				param_nof();
int 			param_get_bsize(char *name);
int 			param_get_bsize_i(int idx);
int				param_get_idx(char *name);
param_t*		param_get(char *name);
param_t*		param_get_i(int idx);
void*			param_get_addr(char *name);
void*			param_get_addr_i(int idx);
int 			param_get_aloe_type(char *name);
int 			param_get_aloe_type_i(int idx);
