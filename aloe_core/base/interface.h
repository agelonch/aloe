#ifndef INTERFACE_H
#define INTERFACE_H

#include "str.h"
#include "manapi_types.h"
#include "packet.h"

/* Inherited methods */
void interface_serialize(interface_t *src, packet_t *pkt);
void interface_unserializeTo(packet_t *pkt, interface_t *dest);

#endif
