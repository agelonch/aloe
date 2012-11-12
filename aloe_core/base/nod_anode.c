#include "defs.h"
#include "hwapi.h"
#include "nod_anode.h"

static nod_anode_t UNUSED(anode);

/**
 * 1) Pre-allocates max_waveforms objects of type Waveform (with max_modules_x_waveform and max_variables_x_module modules and variables object instances) in the +loaded_waveforms array and initialize synchronization master/slave
 * 2) Find physical interface with name "ctrl" and save to ctrlItf
 * 3) Find physical interface with name "probe" and save to probeItf
 * 4) If !hwapi.machine.syncMaster,
 *    4.1) find physical interface with name "sync" and save pointer to syncItf                              
 *    4.2) syncItf.setCallback(SyncSlave,kernelPrio)
 *    4.3) if !hwapi.machine.syncContinuous add periodic function SyncRequest() to the kernel with period "period"
 * 5) If hwapi.machine.syncMaster,
 *    5.1) find all physical interface with name starting with "slavesync*" and save their object address to slaveItf array
 *    5.2) if hwapi.machine.isContinuous add SyncMaster() as a periodic function to the kernel with period 1
 *    5.3) if !hwapi.machine.isContinuous slaveItf[i].setCallback(SyncMaster,kernelPrio) for each slaveItf
 */
int nod_anode_initialize() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * This is a thread created by the hwapi kernel thread with normal priority. Reads commands from ctrl interface and processes them.
 */
int nod_anode_cmd_recv() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * This function is called by a Process object when his module finishes the execution.
 *
 * If process.finishCode!=ProcessErrorCode.OK, an excepcional aerror occurred. The reason is stored in process.errorMsg string. This function then uses the probeItf to send a message to the manager with the format detailed in CmdPlatform.asyncError() method.
 */
int nod_anode_process_finish(h_proc_t *process) {
	aerror("Not yet implemented");
	return -1;
}
