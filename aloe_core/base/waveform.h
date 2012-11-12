#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "objects_max.h"
#include "module.h"
#include "waveform_status.h"

int waveform_update(waveform_t *obj);
void waveform_serialize(waveform_t *dest, packet_t *pkt);
void waveform_unserializeTo(packet_t *pkt, waveform_t *dest);

#endif
