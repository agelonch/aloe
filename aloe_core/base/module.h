#ifndef MODULE_H
#define MODULE_H

#include "str.h"
#include "manapi.h"
#include "packet.h"

int module_update(module_t *obj);
void module_serialize(module_t *dest, packet_t *pkt);
void module_unserializeTo(packet_t *pkt, module_t *dest);

#endif
