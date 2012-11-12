#ifndef WAVEFORM_STATUS_H
#define WAVEFORM_STATUS_H

#include "manapi_types.h"
#include "packet.h"

void waveform_status_serialize(waveform_status_t *dest, packet_t *pkt);
void waveform_status_unserializeTo(packet_t *pkt, waveform_status_t *dest);

#endif


