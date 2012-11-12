#ifndef MAN_DATAITF_H
#define MAN_DATAITF_H

#include "defs.h"
#include "str.h"

typedef struct {
/**
 * A physical data interface connects one node with another. This is the interface used by modules data exchange. Implements Serializable interface and just copies/saves attributes in both methods.
 */

	/**
	 * Unique physical interface id in the node
	 */
	int id;
	/**
	 * Interface address (see platform.conf in CmdPlatform class)
	 */
	strdef(address);
	int remoteItfId;
	int remoteNodeId;

} man_dataitf_t;

#endif
