#include "manapi.h"
#include "str.h"
#include <stddef.h>
#include "defs.h"
#include "waveform.h"
#include "objects_max.h"
#include "waveform.h"
#include "manapi_context.h"
#include "manapi_error.h"
#include "man_platform.h"

manapi_context_t manapi;

/**
 * Indicates if the manager api has been initiated. If already initiated, a call to initialize() will fail.
 */
static int UNUSED(initialized);


/**
 * Creates the CmdPlatform object from the platformConfPath configuration file.
 * call read_waveforms
 */
int manapi_initialize(string platform_conf, string waveforms_repository) {
	man_platform_set_context(&manapi.platform);
	manapi_error_set_context(&manapi.error);

	aerror("Not yet implemented");
	return -1;
}

/**
 * returns the CmdWaveform object associated to the waveform "name"
 */
waveform_t* manapi_waveform_get(string name) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Reads the repository directory and creates initializes the waveform instances objects, one for each waveform model file in the directory (.app extension).
 */
int manapi_read_waveforms(string repository) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * rsh runcf on all nodes
 */
int manapi_start_nodes() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * rsh killcf on all nodes
 */
int manapi_stop_nodes() {
	aerror("Not yet implemented");
	return -1;
}
