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

#include "str.h"

#define INCLUDE_DEFS_ONLY
#include "plp_sink.h"

int plp_init(const char *driver, const char *_title, int is_complex);
void set_legend(char legends[2*NOF_INPUT_ITF][STR_LEN], int nof_legend);
void set_labels(const char *x, const char *y);
int plp_draw(double *signal, int *signal_lengths, int ylog_scale);
void reset_axis();
void plp_destroy();
