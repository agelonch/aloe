#ifndef NOD_ANODE_H
#define NOD_ANODE_H

#include "hwapi.h"
#include "nod_waveform.h"
#include "objects_max.h"

/**
 * This is the class that processes the commands coming from the ALOE-Manager.
 * 
 * The HWAPI runs a background process and creates an object of this class. The HWAPI is responsible to periodically check for incoming messages from the control interface and call the process_cmd() function
 */
typedef struct {
	nod_waveform_t loaded_waveforms[MAX(waveforms)];
	packet_t *packet;
	h_phyitf_t sync_itf;
	h_phyitf_t ctr_iItf;
	h_phyitf_t probe_itf;
	h_phyitf_t slaves_itf[MAX(hwapi_itfphysic)];

} nod_anode_t;

int nod_anode_initialize();
int nod_anode_cmd_recv();
int nod_anode_process_finish();

#endif
