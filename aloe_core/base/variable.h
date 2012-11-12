#ifndef VARIABLE_H
#define VARIABLE_H

#include "str.h"
#include "manapi_types.h"
#include "packet.h"

int variable_update(variable_t *obj);
void variable_serialize(variable_t *dest, packet_t *pkt);
void variable_unserializeTo(packet_t *pkt, variable_t *dest);

#endif
