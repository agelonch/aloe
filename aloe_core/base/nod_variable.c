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

#include "nod_variable.h"

/**
 * Serialize all attributes including variable value (shmPtr) if size>0
 */
void nod_variable_serialize(nod_variable_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

/**
 * Save window and period attributes. If size>0, save variable value to shmPtr while checking size.
 * If new window>0 and period>0, add the variable to module.reportingVariable and increase module.nof_report_vars clear the reportPacket and set periodCnt=0
 * If new window=0 and period=0, remove the variable to module.reportingVariable and decrease module.nof_report_vars
 */
void nod_variable_unserializeTo(packet_t *pkt, nod_variable_t *dest) {
	aerror("Not yet implemented");
}
