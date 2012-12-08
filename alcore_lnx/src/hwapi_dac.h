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

#ifndef HWAPI_DAC_H
#define HWAPI_DAC_H

#include "objects_max.h"

/**
 * This is the interface to the digital converters. Each converter must implement the interface in a particular class
 */
typedef struct {
	int id;
	string options;
	float freq;
	int block_len;
	int sampleType;
	hwapi_itfqueue_t channels[MAX(dac_channel)];
	string address;
	void (*ts_begin_fnc)(void);
	int thread_prio;
	int sync_clock;
} hwapi_dac_t;


int hwapi_dac_open(h_dac_t obj, string address, string options);
int hwapi_dac_close(h_dac_t obj);
int hwapi_dac_set_scheduler(h_dac_t obj, void (*ts_begin_fnc)(void), int thread_prio);
int hwapi_dac_start(h_dac_t obj);
int hwapi_dac_set_opts(h_dac_t obj, string opts);
int hwapi_dac_set_freq(h_dac_t obj, float freq);
int hwapi_dac_set_block_len(h_dac_t obj, int len);
int hwapi_dac_set_sample_type(h_dac_t obj, int type);
int hwapi_dac_set_buffer_sz(h_dac_t obj, int in, int out);
h_itf_t hwapi_dac_channel(h_dac_t obj, int int_ch);

#endif
