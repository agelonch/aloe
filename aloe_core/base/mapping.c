#include "defs.h"
#include "mapping.h"
#include "man_platform.h"
#include "nod_waveform.h"

extern man_platform_t UNUSED(platform);

/**
 * 1) Configure the mapping algorithm
 * 2) generate the c,b waveform model
 * 3) call the mapping algorithm with the man_platform_model() and waveform modules
 * 4) for each module, assign its node.
 * 5) for all modules's interfaces, assign physical interfaces id (if communication is external)
 * 6) construct the result[] array of waveform objects
 */
int mapping_map(nod_waveform_t *waveform) {
	aerror("Not yet implemented");
	return -1;
}
