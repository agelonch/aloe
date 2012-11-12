#ifndef SWAPI_VAR_H
#define SWAPI_VAR_H

#include "nod_variable.h"

/**
 * A non-instantiable class (no attributes). Just a collection of functions associated to variable management.
 */

typedef struct {
	int id;
	nod_variable_t variable;
}swapi_var_t;

#endif
