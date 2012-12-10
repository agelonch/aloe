/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "packet.h"
#include "defs.h"

/* in a multi-processing environment, this file wont be included */
#include "nod_anode.h"


extern packet_t *man_packet;
extern packet_t *node_packet;

#define assert_init if (!pkt->buffer) return -1
#define assert_init_p if (!pkt->buffer) return NULL



/**  Creates a packet of size bytes length
 * \returns 0 on success, -1 on error
 */
int packet_init(packet_t *pkt, int size) {
	pktdebug("size=%d\n",size);
	aassert(pkt);
	pkt->buffer = malloc((size_t) size);
	if (!pkt->buffer) {
		return -1;
	}
	pkt->size = size;
	return 0;
}

/**  Clears the packet contents
 * \returns 0 on success, -1 on error
 */
int packet_clear(packet_t *pkt) {
	pktdebug("w_ptr=%d, r_ptr=%d\n",pkt->w_ptr,pkt->r_ptr);
	assert_init;
	aassert(pkt);
	memset(&pkt->dest, 0, sizeof(packet_dest_t));
	pkt->r_ptr = 0;
	pkt->w_ptr = 0;
	return 0;
}

/**
 *  Reads the packet header. This function must be called at reception before calling get_cmd()
 * or get_data().
 */
int packet_read(packet_t *pkt) {
	pktdebug("w_ptr=%d, r_ptr=%d\n",pkt->w_ptr,pkt->r_ptr);
	assert_init;
	aerror("Not yet implemented");
	return -1;
}

/**
 *  Defines the command field on the packet header.
 */
int packet_set_cmd(packet_t *pkt, packet_command_t cmd) {
	pktdebug("cmd=%d\n",cmd);
	assert_init;
	aassert(pkt);
	memcpy(&pkt->cmd,&cmd,sizeof(packet_command_t));
	return 0;
}

/**
 *  returns the command field from the packet header.
 */
packet_command_t packet_get_cmd(packet_t *pkt) {
	assert_init;
	assert(pkt);
	return pkt->cmd;
}

/**  Adds len bytes from buffer pointed by data to the packet.
 * \returns 0 on success, -1 on error because invalid parameter or insufficient space in the packet
 */
int packet_add_data(packet_t *pkt, void* data, int len) {
	pktdebug("w_ptr=%d, r_ptr=%d, data=0x%x, len=%d\n",pkt->w_ptr,pkt->r_ptr, data, len);
	assert_init;
	aassert(pkt);
	aassert(data);
	aassert(len>=0);
	if (pkt->w_ptr + len > pkt->size) {
		return -1;
	}
	memcpy(&pkt->buffer[pkt->w_ptr], data, (size_t) len);
	pkt->w_ptr += len;

	return 0;
}

/**
 *  Copies len bytes of data from the packet to the buffer pointed by data.
 * \returns 0 on success or -1 on error due to invalid parameters or insufficient data
 */
int packet_get_data(packet_t *pkt, void* data, int len) {
	pktdebug("w_ptr=%d, r_ptr=%d, data=0x%x, len=%d\n",pkt->w_ptr,pkt->r_ptr, data, len);
	assert_init;
	aassert(pkt);
	aassert(data);
	aassert(len>0);

	if (pkt->w_ptr - pkt->r_ptr < len) {
		return -1;
	}
	memcpy(data, &pkt->buffer[pkt->r_ptr], (size_t) len);
	pkt->r_ptr += len;

	return 0;
}

/**  returns the number of bytes written to the packet
 */
int packet_total_size(packet_t *pkt) {
	pktdebug("w_ptr=%d, r_ptr=%d\n",pkt->w_ptr,pkt->r_ptr);
	assert_init;
	aassert(pkt);
	return pkt->w_ptr;
}

/**  returns the number of unreaded bytes in the packet
 *
 */
int packet_toread_size(packet_t *pkt) {
	pktdebug("w_ptr=%d, r_ptr=%d\n",pkt->w_ptr,pkt->r_ptr);
	assert_init;
	aassert(pkt);
	return pkt->w_ptr-pkt->r_ptr;
}

/**  sets the packet ACK word
 *
 */
int packet_set_ack(packet_t *pkt, int ack) {
	pktdebug("ack=%d\n",ack);
	assert_init;
	aassert(pkt);
	pkt->ack = ack;
	return 0;
}

/**  returns the packet ACK word
 *
 */
int packet_get_ack(packet_t *pkt) {
	assert_init;
	return pkt->ack;
}

/**  Returns a pointer to the packet destination structure
 *
 */
packet_dest_t *packet_get_dest(packet_t *pkt) {
	assert_init_p;
	return &pkt->dest;
}

/**  Sends the packet to a destination node (dest->node) and object id 3-tuple
 * (waveform_id, module_id, variables_id) configured in the dest structure.
 * \returns 0 on success or -1 on error
 */
int packet_sendto(packet_t *pkt, packet_dest_t *dest) {
	int ack;
	pktdebug("node=0x%x, dest=(%d,%d,%d)\n",dest->node,dest->waveform_id,
			dest->module_id, dest->variable_id);
	assert_init;
	aassert(pkt);
	aassert(dest);

	packet_clear(node_packet);

	packet_set_cmd(node_packet, pkt->cmd);
	memcpy(&node_packet->dest, dest, sizeof(packet_dest_t));
	if (packet_add_data(node_packet, pkt->buffer, packet_total_size(pkt)))
		return -1;

	/**@TODO: In multi-platform mode, this should wait for the packet reception
	 */
	ack = nod_anode_cmd_recv();

	pkt->ack = ack;

	/* copy the packet back */
	if (packet_add_data(pkt, node_packet->buffer, packet_total_size(node_packet)))
		return -1;

	return 0;
}

