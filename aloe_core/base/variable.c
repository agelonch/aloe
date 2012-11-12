#include "variable.h"
#include "packet.h"

/**
 * Gets the variable of the module running in an ALOE-node and saves the value to the buffer "value". Only min("size",variable.size) bytes will be updated.
 * 1) Verify module.waveform.Status=RUN
 * 2) Send Cmd.GET dest(variable.module.waveform.Id, variable.module.Id, variable.Id) to variable.module.node using variable.module.waveform.cmdPlatform
 * 3) wait for answer and serializeTo(variable) the packet
 * 4) update lastUpdateTS with current ts
 */
int variable_update(variable_t *obj) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Begins a continuous report of a variable value with a given period and window. Each timeslot, the module produces a new variable value, which is received by the manager who calls the function pointed by "fncPtr".
 * 1) Verify module.waveform.Status.isRunning()
 * 2) Use module.node.probeListener.addCallback(fncPtr, variable.Id) to setup a callback handler to the Probe physical interface.
 * 3) save window and period attributes. Save size to temporal variable. Set size to 0.
 * 4) Send Cmd.SET dest(variable.module.waveform.Id, variable.module.Id, variable.Id) to module.node using module.waveform.cmdPlatform and serialize(variable)
 * 5) Restore size from temporal variable.
 * 6) Waits for ACK. If OK quits otherwise removes the callback using module.node.probeListener.removeCallback(variable.Id)
 */
int variable_report_start(variable_t *var, void (*callback) (void), int period, int window) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Stops reporting a variable. Sets window=period=0 and serializes de variable using SET command. The variable then not generate any other packet. This function then removes the callback from module.node.probeListener
 */
int variable_report_stop(variable_t *var) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Serialize all attributes including value if size>0
 */
void variable_serialize(variable_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

/**
 * Save all attributes (including curValue if size>0) except initValue
 */
void variable_unserializeTo(packet_t *pkt, variable_t *dest) {
	aerror("Not yet implemented");
}
