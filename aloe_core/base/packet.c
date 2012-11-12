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

#include <stddef.h>
#include "packet.h"

/**
 * Clears the packet contents and its header (including command type)
 */
int packet_clear(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Reads the packet header. This function must be called at reception before calling get_cmd() or get_data().
 */
int packet_read(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Returns a pointer to the packet buffer (for transmission and reception through the interface functions)
 */
void* packet_buffer(packet_t *pkt) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Defines the command field on the packet header.
 */
int packet_set_cmd(packet_t *pkt, packet_command_t cmd) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * returns the command field from the packet header.
 */
packet_command_t packet_get_cmd(packet_t *pkt) {
	aerror("Not yet implemented");
	return 0;
}

/**
 * Appends "len" bytes from "data" pointer to the end of the packet unless there is not enough space in the packet. Returns the number of bytes that where successfully appended.
 */
int packet_add_data(packet_t *pkt, void* data, int len) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Copies "len" bytes of the packet data to the buffer pointed by "buffer" unless the packet has no more data to read. Returns the number of bytes that where successfully copied.
 */
int packet_get_data(packet_t *pkt, void* buffer, int len) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Returns the maximum size of the packet (in bytes)
 */
int packet_max_size(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Returns the current size of the packet (in bytes)
 */
int packet_size(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

int packet_set_ack(packet_t *pkt, int ack) {
	aerror("Not yet implemented");
	return -1;
}

int packet_get_ack(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * returns the packet header
 */
int packet_get_header(packet_t *pkt) {
	aerror("Not yet implemented");
	return -1;
}
