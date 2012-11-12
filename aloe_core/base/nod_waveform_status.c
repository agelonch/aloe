#include "nod_waveform_status.h"
#include "packet.h"

/**
 * Copy all attributes
 */
void nod_waveform_status_serialize(nod_waveform_status_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

/**
 * This function is called when a new status change has been indicated. This unserialize function is a bit more complex, since it must ensure that the status has been changed successfully.
 *
 * 1) Copy all attributes
 * 2) call hwapi.sleepToTstamp(dead_tstamp) to sleep until dead_tstamp
 * 3) when back, return OK if all modules have the correct status. Return ERR otherwise
 */
void nod_waveform_status_unserializeTo(packet_t *pkt, nod_waveform_status_t *dest) {
	aerror("Not yet implemented");
}
