#include "defs.h"
#include "man_probelistener.h"

int man_probelistener_add(man_probelistener_t *lstnr, void (*callback)(void), int code) {
	aerror("Not yet implemented");
	return -1;
}

int man_probelistener_remove(man_probelistener_t *lstnr, int code) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Sets process() function as a callback to the interface
 *  
 */
int man_probelistener_setup(man_probelistener_t *lstnr, h_itf_t *itf) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Called when a new packet arrives, receives it from the interface and uses the CmdPacket interface to read the first 32-bit word of the packet, find the associated handler and call it, passing the packet data pointer and size as first and second parameters.
 */
int man_probelistener_process(man_probelistener_t *lstnr) {
	aerror("Not yet implemented");
	return -1;
}
