#ifndef SWAPI_ITF_H
#define SWAPI_ITF_H

#include "swapi.h"
#include "hwapi.h"

/**
 * Class for inter-module data interface management and utilization.
 */

typedef struct {
	int id;
	h_itf_t hwitf;
}swapi_itf_t;

int swapi_itf_init(itf_t itf);

#endif
