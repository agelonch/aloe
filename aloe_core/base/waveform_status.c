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

#include "waveform_status.h"

int waveform_status_to_string(waveform_status_t *status) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Modifies the waveform execution status. The status is first modified locally and, if the transition is valid, a command to the ALOE-Nodes is issued:
 * 1) Verify isLoaded()
 * 2) Check if the status transition is valid.
 * 3) Iterate through the waveform.mapping.node array and send command Cmd.SET to dest(waveform.Id,0,0) and serialize(waveform.Status)
 * 4) wait for reception positive ACK
 * 5) If nextStatus=Status.INIT call waveform.update()
 */
int waveform_status_set(waveform_status_t *status, waveform_status_enum new_status, int next_ts, int dead_ts) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * returns true if the Status is LOADED, INIT, RUN or PAUSE
 */
int waveform_status_is_loaded(waveform_status_t *status) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * returns true if the waveform is in status RUN or PAUSE
 */
int waveform_status_is_running(waveform_status_t *status) {
	aerror("Not yet implemented");
	return -1;
}

void waveform_status_serialize(waveform_status_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

void waveform_status_unserializeTo(packet_t *pkt, waveform_status_t *dest) {
	aerror("Not yet implemented");
}

