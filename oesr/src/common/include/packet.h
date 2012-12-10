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

typedef enum {
	CMD_LOAD, CMD_SET, CMD_GET, CMD_CONNECT, CMD_DISCONNECT, CMD_HWINFO
} packet_command_t;

typedef struct {
	int waveform_id;
	int module_id;
	int variable_id;
	void *node;
}packet_dest_t;

typedef struct {
	packet_command_t cmd;
	packet_dest_t dest;
	int ack;
	int w_ptr;
	int r_ptr;
	int size;
	char *buffer;
} packet_t;

int packet_init(packet_t *packet, int size);
int packet_clear(packet_t *packet);
int packet_set_cmd(packet_t *packet, packet_command_t cmd);
packet_command_t packet_get_cmd(packet_t *packet);
int packet_add_data(packet_t *packet, void* data, int len);
int packet_get_data(packet_t *packet, void* buffer, int len);
int packet_total_size(packet_t *packet);
int packet_toread_size(packet_t *packet);
int packet_set_ack(packet_t *packet, int ack);
int packet_get_ack(packet_t *packet);
packet_dest_t *packet_get_dest(packet_t *packet);

int packet_sendto(packet_t *packet, packet_dest_t *dest);
int packet_read(packet_t *packet);

#define add_i(a) if (packet_add_data(pkt,a,sizeof(int))) return -1;
#define get_i(a) if (packet_get_data(pkt,a,sizeof(int))) return -1;

#endif
