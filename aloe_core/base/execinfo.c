
#include "modexecinfo.h"

/**
 * Updates the local object's attributes with the node's attributes.
 * 1) Verify mmodule.waveform.Status.isLoaded()
 * 2) Send Cmd.GET dest(module.waveform.Id, module.Id,0) to module.node using module.waveform.cmdPlatform
 * 3) wait for answer and serializeTo(module.execInfo)
 * 4) update lastUpdateTS with current ts
 */
int execinfo_update(execinfo_t *obj) {
	aerror("Not yet implemented");
	
	return -1;
}

void execinfo_serialize(execinfo_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

void execinfo_unserializeTo(packet_t *pkt, execinfo_t *dest) {
	aerror("Not yet implemented");
}
