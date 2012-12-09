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

#ifndef rtdal_DACX5_H
#define rtdal_DACX5_H

#include "str.h"
#include "rtdal_dac.h"
#include "rtdal.h"

/**
 * Implementation of the dacx5 interface to the Universal Hardware Driver (x5) for USRP family of devices.
 */
typedef struct {
	rtdal_dac_t parent;
}rtdal_dacx5_t;


int rtdal_dacx5_open(h_dac_t obj, string address, string options);
int rtdal_dacx5_close(h_dac_t obj);
int rtdal_dacx5_set_scheduler(h_dac_t obj, void (*ts_begin_fnc)(void), int thread_prio);
int rtdal_dacx5_start(h_dac_t obj);
int rtdal_dacx5_set_opts(h_dac_t obj, string opts);
int rtdal_dacx5_set_freq(h_dac_t obj, float freq);
int rtdal_dacx5_set_block_len(h_dac_t obj, int len);
int rtdal_dacx5_set_sample_type(h_dac_t obj, int type);
int rtdal_dacx5_set_buffer_sz(h_dac_t obj, int in, int out);
h_itf_t rtdal_dacx5_channel(h_dac_t obj, int int_ch);

#endif
