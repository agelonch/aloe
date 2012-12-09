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

#ifndef rtdal_DAC_H
#define rtdal_DAC_H

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
	rtdal_itfspscq_t channels[MAX(dac_channel)];
	string address;
	void (*ts_begin_fnc)(void);
	int thread_prio;
	int sync_clock;
} rtdal_dac_t;


int rtdal_dac_open(r_dac_t obj, string address, string options);
int rtdal_dac_close(r_dac_t obj);
int rtdal_dac_set_scheduler(r_dac_t obj, void (*ts_begin_fnc)(void), int thread_prio);
int rtdal_dac_start(r_dac_t obj);
int rtdal_dac_set_opts(r_dac_t obj, string opts);
int rtdal_dac_set_freq(r_dac_t obj, float freq);
int rtdal_dac_set_block_len(r_dac_t obj, int len);
int rtdal_dac_set_sample_type(r_dac_t obj, int type);
int rtdal_dac_set_buffer_sz(r_dac_t obj, int in, int out);
r_itf_t rtdal_dac_channel(r_dac_t obj, int int_ch);

#endif
