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

#ifndef MANAPI_WAVEFORM_H_
#define MANAPI_WAVEFORM_H_

#include "str.h"
#include "manapi_module.h"
#include "manapi_waveform_status.h"

/**
 * Inherits from General.Waveform class. After the waveform is loaded to a platform, the class allows to send status change commands and update the local objects with the remote ALOE-Node waveform objects. A waveform must be parsed (parse()) prior to being loaded (load()).
 *
 * Sample waveform configuration file:
 * ==========================================
 *
 * modules:
 * {
 *
 * convcoder:
 * {
 *         binary="convcoder1.0.so";
 *         proc=XXX // in mopts
 *
 *         outputs=(
 *          ( "output1", "decoder", "input1", 0.3),  localItfName, remoteModuleName, remoteItfName, BW requirements (in mbpts)
 *          ( "output2", "otherMod", "output2", 1)
 *          );
 *
 *         variables=(
 *          (name="code"; value=[0x79,0x10,0xf4];),
 *          (name="blocksize"; value=100;)
 *          );
 * };
 *
 *
 * decoder:
 * {
 *         binary="decoder1.2.so";
 *         proc=XXX // in mopts
 *
 *         inputs=( "input1", "convcoder", "output1");  Inputs do not specify BW. Must coincide with output interface  
 *         outputs=( "output1", "sink", "input1", 0.1);
 *
 *         variables=(
 *          (name="code"; value=[0x79,0x10,0xf4];),
 *          (name="blocksize"; value=100;),
 *          (name="SNR"; value=3.2;)
 *          );
 * };
 *
 * decoder:
 * {
 *         binary="decoder1.2.so";
 *         proc=XXX // in mopts
 *         inputs=( "input1", "decoder", "output1");
 * };
 *
 * options:
 * {
 *         max_delay_us=2000;         This is currently not in use
 *         pre_mapping=(
 *                 ( "decoder", "sink"),
 *                 (  another vector of joined modules ));
 * }
 */

typedef struct {

	module_t modules[MAX(modules)];
	waveform_status_t status;
	/**
	 * Timeslot of the last update
	 */
	int last_update_ts;
	/**
	 * Number of timeslots to delay an status change. Should be larger than the maximum delay (in timeslots) to any Node
	 */
	int status_timestamp_delay;
	/**
	 * vector of computing requirements, in mopts
	 */
	float c[MAX(modules)];
	/**
	 * matrix of bandwidth requirements, in mbpts
	 */
	float b[MAX(modules)];
	int nof_modules;
	int mapped_nodes_id[MAX(nodes)];
	strdef(model_file);

} waveform_t;


#endif
