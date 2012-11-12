#ifndef MAN_NODE_H
#define MAN_NODE_H

#include "man_dataitf.h"
#include "man_processor.h"
#include "man_probelistener.h"
#include "objects_max.h"
#include "hwapi.h"

/**
 * A node is a collection of processors sharing memory. It may have data interfaces to other nodes. It implements Serializable interface and just copies all attributes
 */

typedef struct {

	void *platform;
	man_processor_t man_processors[MAX(processors)];
	man_dataitf_t man_dataitfs[MAX(hwapi_itfphysic)];
	man_probelistener_t probe_listener;
	h_phyitf_t* ctr_itf;
	h_phyitf_t* probe_itf;

	/**
	 * Name of the node, as specified in platform.conf in CmdPlatform class
	 */
	strdef(name);
	int id;

} man_node_t;

#endif
