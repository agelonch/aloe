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

#include <unistd.h>
#include <stdlib.h>
#include <float.h>

#include <aloe/swapi.h>
#include <aloe/skeleton.h>
#include "plplot.h"
#include "plevent.h"
#include "plp.h"

#define INCLUDE_DEFS_ONLY
#include "plp_sink.h"

static PLFLT x[2*NOF_INPUT_ITF][INPUT_MAX_SAMPLES], t[INPUT_MAX_SAMPLES];
static PLFLT xmin, xmax, ymin, ymax;

static int is_complex;
static int nof_legends;

const char *legends[2*NOF_INPUT_ITF], *xlabel, *ylabel, *title;

const char xlabel_def[] = "x";
const char ylabel_def[] = "y";

PLINT       opt_array[2*NOF_INPUT_ITF];
PLINT       text_colors[2*NOF_INPUT_ITF];
PLINT        line_colors[2*NOF_INPUT_ITF];
PLINT        line_styles[2*NOF_INPUT_ITF];
PLINT        line_widths[2*NOF_INPUT_ITF];
PLINT        symbol_colors[2*NOF_INPUT_ITF];
PLFLT        symbol_scales[2*NOF_INPUT_ITF];
PLINT        symbol_numbers[2*NOF_INPUT_ITF];
const char   *symbols[2*NOF_INPUT_ITF];
PLFLT        legend_width, legend_height;

const char axis_x[] = "bcnst";
const char axis_y[] = "bcnstv";
const char logaxis_x[] = "bcnstl";
const char logaxis_y[] = "bcnstvl";

void setup_legend();

int plp_init(const char *driver, const char *_title, int _is_complex) {
	int i;

	if (2*NOF_INPUT_ITF > 1) {
		modinfo("Multiple signals are currently displayed in the same plot\n");
	}

	plsdev(driver);
	plinit();
	pladv(0);

	title = _title;
	is_complex = _is_complex;

	for (i=0;i<2*NOF_INPUT_ITF;i++) {
		legends[i] = (const char*) "";
	}

	for (i=0;i<INPUT_MAX_SAMPLES;i++) {
		t[i] = i;
	}

	setup_legend();

	xlabel = xlabel_def;
	ylabel = ylabel_def;

	plscol0a( 14, 0, 0, 0, 1);

	reset_axis();
	return 0;
}

void plp_destroy() {
	plend();
}

void reset_axis() {
	xmin = 0;
	xmax = -1;
	ymin = FLT_MAX;
	ymax = -FLT_MAX;
}

void set_labels(const char *x, const char *y) {
	xlabel = x;
	ylabel = y;
}

void setup_legend() {
	int i;
	int col=3;
	for (i=0;i<2*NOF_INPUT_ITF;i++) {
		opt_array[i]   = PL_LEGEND_LINE;
		text_colors[i] = 14;
		line_colors[i] = col;
		col++;
		if (col == 4) col++;
		line_styles[i] = 1;
		line_widths[i] = 4;
		symbol_colors[i] = 0;
		symbol_scales[i] = 0;
		symbol_numbers[i] = 0;
		symbols[i] = "*";
	}
}
void set_legend(char _legends[2*NOF_INPUT_ITF][STR_LEN], int _nof_legends) {
	int i;
	nof_legends = _nof_legends;
	for (i=0;i<nof_legends;i++) {
		legends[i] = (const char*) _legends[i];
	}
}

void draw_legend() {
	PLFLT legend_width, legend_height;

	pllegend( &legend_width, &legend_height,
		PL_LEGEND_BOUNDING_BOX, PL_POSITION_TOP | PL_POSITION_OUTSIDE,
		0.0, 0.02, 0.1, 0,
		14, 1, 0, nof_legends,
		nof_legends, opt_array,
		1.0, 1.0, 2.0,
		0., text_colors, legends,
		NULL, NULL, NULL, NULL,
		line_colors, line_styles, line_widths,
		symbol_colors, symbol_scales, symbol_numbers, (const char**) symbols);
}

int plp_draw(double *signal, int *signal_lengths, int ylog_scale) {
	int count;
	int i,j;
	int col;

	for (i=0;i<2*NOF_INPUT_ITF;i++) {
		if (signal_lengths[i] > INPUT_MAX_SAMPLES) {
			moderror_msg("plplot buffer configured for %d samples but received %d in signal %d\n",
					INPUT_MAX_SAMPLES,signal_lengths[i],i);
			return -1;
		}
	}
	xmax=0;
	for(i=0;i<2*NOF_INPUT_ITF;i++) {
		xmax = (PLFLT) MAX(xmax,signal_lengths[i]);
		for (j=0;j<signal_lengths[i];j++) {
			ymin = (PLFLT) MIN(ymin,signal[i*INPUT_MAX_SAMPLES+j]);
			ymax = (PLFLT) MAX(ymax,signal[i*INPUT_MAX_SAMPLES+j]);
		}
	}

	plclear();

	plscolbg(255, 255, 255);
	plvsta();
	plwid(1);
	plwind(xmin, xmax, ymin*1.1, ymax*1.1);
	plcol0(1);
	if (ylog_scale) {
		plbox(logaxis_x, 0., 0, logaxis_y, 0., 0);
	} else {
		plbox(axis_x, 0., 0, axis_y, 0., 0);
	}
	plcol0(4);
	plbox("g", 0, 0, "g", 0, 0);
	plcol0(1);
	pllab(xlabel, ylabel, "");

	draw_legend();

	plwid(4);
	col=3;
	for (i=0;i<2*NOF_INPUT_ITF;i++) {
		if (signal_lengths[i]) {
			plcol0(line_colors[i]);
			plline(signal_lengths[i], t, &signal[i*INPUT_MAX_SAMPLES]);
			col++;
			if (col==4) col++;
		}
	}

	plflush();                      // force an update of the tk driver

}

