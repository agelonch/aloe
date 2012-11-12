#ifndef MAPPING_H
#define MAPPING_H

#include "objects_max.h"

/**
 * This class runs the waveform-to-platform mapping algorithm and stores the result. The mapping procedure is as follows:
 * 1) Create an object instance
 * 2) Setup algorithm and options
 * 3) call to map(platform,waveform) to map "waveform" object to "platform" object
 * 4) Result is stored in p_res and cost fields
 */

typedef struct {

	/**
	 * Position i of the vector gives the processor index the i-th module has been mapped to
	 */
	int p_res[MAX(modules)];
	float cost;

	int algorithm;
	int options;

} mapping_t;

#endif
