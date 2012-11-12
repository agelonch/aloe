
#ifndef MANAPI_INTERFACE_H_
#define MANAPI_INTERFACE_H_

#include "str.h"

typedef struct {
	/**
 * Data interface between modules.
 */

	/**
	 * Unique id in the waveform
	 */
	int id;
	/**
	 * interface bandwidth requirements in MBPTS
	 */
	float bw_total_mbpts;
	/**
	 * Used bandwidth (in MBPTS). *NOTE*: Unlike module's ExecInfo, the interfaces are assumed to use constant bandwidth.
	 */
	float bw_use_mbpts;
	/**
	 * Name of the interface
	 */
	strdef(name);
	strdef(remoteItfName);
	strdef(remoteObjName);
	int remoteItfId;
	int remoteObjId;
	int phyItfId;
	/**
	 * Interface delay, in timeslots
	 */
	int delay;
	int active;

} interface_t;

#endif
