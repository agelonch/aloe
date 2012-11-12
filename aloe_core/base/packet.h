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

#ifndef PACKET_H
#define PACKET_H

#include "man_node.h"

typedef enum {
	LOAD, SET, GET, CONNECT, DISCONNECT, HWINFO
} packet_command_t;

typedef struct {
	int waveform_id;
	int module_id;
	int variable_id;
	man_node_t *node;
}packet_dest_t;

typedef struct {
	packet_command_t cmd;
	packet_dest_t dest;
	/**
	 * Size of the packet buffer
	 */
	int buffer_sz;
	/**
	 * Packet write pointer
	 */
	int w_ptr;
	/**
	 * Packet read pointer
	 */
	int r_ptr;

} packet_t;

int packet_clear(packet_t *packet);
int packet_read(packet_t *packet);
void* packet_buffer(packet_t *packet);
int packet_set_cmd(packet_t *packet, packet_command_t cmd);
packet_command_t packet_get_cmd(packet_t *packet);
int packet_add_data(packet_t *packet, void* data, int len);
int packet_get_data(packet_t *packet, void* buffer, int len);
int packet_max_size(packet_t *packet);
int packet_size(packet_t *packet);
int packet_set_ack(packet_t *packet, int ack);
int packet_get_ack(packet_t *packet);
int packet_get_header(packet_t *packet);

#endif
