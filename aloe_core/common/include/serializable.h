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


#include "packet.h"
#include "waveform.h"

/** Indicates which data shall be serialized/unserialized to/from the packet.
 * NONE: copies only the variable names; CP_VALUE copies the current value;
 * CP_INIT copies the initializing value
 */
enum variable_serialize_data {
	NONE, CP_VALUE, CP_INIT
};

int waveform_serialize(waveform_t *dest, packet_t *pkt, int loading_node_id,
		enum variable_serialize_data copy_data);
int waveform_unserializeTo(packet_t *pkt, waveform_t *dest,
		enum variable_serialize_data copy_data);

int waveform_mode_serialize(waveform_mode_t *src, packet_t *pkt);
int waveform_mode_unserializeTo(packet_t *pkt, waveform_mode_t *dest);

int variable_serialize(variable_t *dest, packet_t *pkt, enum variable_serialize_data copy_data,
		int nof_modes);
int variable_unserializeTo(packet_t *pkt, variable_t *dest, enum variable_serialize_data copy_data,
		int nof_modes);

int module_serialize(module_t *dest, packet_t *pkt, enum variable_serialize_data copy_data);
int module_unserializeTo(packet_t *pkt, module_t *dest, enum variable_serialize_data copy_data);

int execinfo_serialize(execinfo_t *src, packet_t *pkt);
int execinfo_unserializeTo(packet_t *pkt, execinfo_t *dest);

int interface_serialize(interface_t *src, packet_t *pkt);
int interface_unserializeTo(packet_t *pkt, interface_t *dest);

int waveform_status_serialize(waveform_status_t *dest, packet_t *pkt);
int waveform_status_unserializeTo(packet_t *pkt, waveform_status_t *dest);
