#ifndef NOD_WAVEFORM_STATUS
#define NOD_WAVEFORM_STATUS

#include "waveform_status.h"
#include "packet.h"

typedef struct {
	waveform_status_t parent;
} nod_waveform_status_t;

void nod_waveform_status_serialize(nod_waveform_status_t *src, packet_t *pkt);
void nod_waveform_status_unserializeTo(packet_t *pkt, nod_waveform_status_t *dest);

#endif

