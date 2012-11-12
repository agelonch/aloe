#include "packet.h"
#include "nod_dispatcher.h"

int nod_dispatcher_dispatch(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}
/**
 * The process creation is slightly complex in order to correctly isolate the hwapi from the swapi and node functionality. Therefore, the hwapi.Process class has no information on what a pid or a Node/Module class is, and what are they used for. Conversely, the Process.startup() call accepts a pointer argument which are passed to the swapi, who does know what to do with it (actually, it is a pointer to the module object).
 *
 *
 * 1) Find in +loaded_waveforms the first object with Status=STOP
 * 2) unserializeTo() this object
 * 3) for each module in the waveform, find an empty Process object in hwapi.procs array
 *   3.a) Set binaryPath, execPosition, processorIdx and Module in Process attributes
 *   3.b) Set finishCallback function pointer to processError
 *   3.c) Save Process pointer to moduleObj.process
 *   3.d) call to moduleObj.pid=hwapi.procs[i].create() method
 *   3.e) Initialize hwapi.procs[i].swapi_context structure. Set module pointer
 * 4) Set the waveform status to LOADED
 * 5) send ACK ok
 */
int nod_dispatcher_load(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}
/**
 * 1) Use processing_packet.getDest() to get the destination object
 * 2) Find waveformId
 * 3) If moduleId=0, unserializeTo(waveform.status) and return OK
 * 4) Find moduleId
 * 5) If variableId=0 do nothing (nothing implemented) return ERR
 * 6) Find variableId, unserializeTo(waveform.module.variable) and return OK
 *
 * If at any stage, the Id is not found, return ERR
 */
int nod_dispatcher_set(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) Use processing_packet.getDest() to get the destination object
 * 2) If waveformId=0, serialize() node and return OK
 * 3) Find waveformId
 * 4) If moduleId=0, serialize() waveform and return OK
 * 5) Find moduleId
 * 6) If variableId=0 serialize() waveform.module.execInfo and return OK
 * 7) Find variableId, serialize() waveform.module.variable and return OK
 *
 * If at any stage, the Id is not found, return ERR
 */
int nod_dispatcher_get(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) Connect all output data interfaces
 */
int nod_dispatcher_connect_node() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) Disconnect all output interfaces
 */
int nod_dispatcher_disconnect_node() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Copies the contents of the hwapi.machine structure to the packet
 */
int nod_dispatcher_hwinfo(packet_t pkt) {
	aerror("Not yet implemented");
	return -1;
}
