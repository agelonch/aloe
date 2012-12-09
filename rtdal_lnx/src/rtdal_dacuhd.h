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

#ifndef rtdal_DACUHD_H
#define rtdal_DACUHD_H

#include "str.h"
#include "rtdal_dac.h"
#include "rtdal.h"

/**
 * Implementation of the dacuhd interface to the Universal Hardware Driver (UHD) for USRP family of devices.
 */
typedef struct {
	rtdal_dac_t parent;
}rtdal_dacuhd_t;

int rtdal_dacuhd_open(r_dac_t obj, string address, string options);
int rtdal_dacuhd_close(r_dac_t obj);
int rtdal_dacuhd_set_scheduler(r_dac_t obj, void (*ts_begin_fnc)(void), int thread_prio);
int rtdal_dacuhd_start(r_dac_t obj);
int rtdal_dacuhd_set_opts(r_dac_t obj, string opts);
int rtdal_dacuhd_set_freq(r_dac_t obj, float freq);
int rtdal_dacuhd_set_block_len(r_dac_t obj, int len);
int rtdal_dacuhd_set_sample_type(r_dac_t obj, int type);
int rtdal_dacuhd_set_buffer_sz(r_dac_t obj, int in, int out);
r_itf_t rtdal_dacuhd_channel(r_dac_t obj, int int_ch);

#endif
