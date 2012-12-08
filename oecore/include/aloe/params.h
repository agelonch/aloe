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

/* Note: The parameter type must coincide (the order) with swapi_var_type_t in swapi_types.h */

/** Parameter type */
typedef enum {INT, FLOAT, STRING} param_type_t;

typedef void* pmid_t;


/**\brief Stores in ptr up to max_size elements of the value of the parameter with id the value
 * returned by the function param_id().
 *
 * Sets the number of written elements to the pointer size and the parameter type to "type".
 *
 * The following format for parameter values is assumed:
 *    - *.app file (vectors are comma-separted values):
 *    	- float: Number with a decimal point "."
 *    	- int: Number with no decimal point or hexadecimal number (begining with 0x)
 *    	- string: Character string
 *
 *    - standalone arguments (vector values not yet supported):
 *    	- float: Number with a decimal point "."
 *    	- int: Number with no decimal point or hexadecimal number (begining with 0x)
 *    	- string: Anything else
 *
 *    - Matlab/Octave (vector values not yet supported):
 *    	- float: Default matlab/octave numeric value
 *    	- int: Matlab/octave int32 type (e.g. a=int32(x))
 *    	- string: Not supported
 *
 *  \returns -1 on error or a non-negative integer indicating the number of written bytes
 *  on success
 */
int param_get(pmid_t id, void *ptr, int max_size, param_type_t *type);


/**\brief Returns a positive integer identifying the parameter name. The functions
 * param_get_int_id() and param_get_float_id() can then be used as the functions
 * param_get_int() or param_get_float() to obtain faster access to the parameter values.
 * Useful if the module needs to obtain the updated values continuously.
 */
pmid_t param_id(char *name);


int param_get_int(pmid_t id, int *value);
int param_get_float(pmid_t id, float *value);
int param_get_int_name(char *name, int *value);
int param_get_float_name(char *name, float *value);


/** this interface is deprecated */
#ifdef _ALOE_OLD_SKELETON
void *param_get_addr(char *name);
#endif









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



/** Parameter object */
typedef struct {
	char *name;
	param_type_t type;
	int size;
}param_t;
