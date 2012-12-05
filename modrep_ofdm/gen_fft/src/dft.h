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
 
#include <fftw3.h>

typedef enum {
	COMPLEX_2_COMPLEX, REAL_2_REAL, COMPLEX_2_REAL
}dft_mode_t;

typedef enum {
	FORWARD, BACKWARD
}dft_dir_t;

#define DFT_MIRROR		0b10
#define DFT_PSD			0b100
#define DFT_OUT_DB		0b1000
#define DFT_NORMALIZE   0b10000

typedef struct {
	int size;
	int sign;
	void *in;
	void *out;
	void *p;
	int options;
	dft_dir_t dir;
	dft_mode_t mode;
}dft_plan_t;

typedef _Complex float dft_c_t;
typedef float dft_r_t;

int dft_plan_vector(const int *dft_points, dft_mode_t *modes, dft_dir_t *dirs,
		int nof_plans, dft_plan_t *plans);

int dft_plan_multi_c2c(const int *dft_points, dft_dir_t dir, int nof_plans,
		dft_plan_t *plans);
/*... */

int dft_plan(const int dft_points, dft_mode_t mode, dft_dir_t dir, dft_plan_t *plan);
int dft_plan_c2c(const int dft_points, dft_dir_t dir, dft_plan_t *plan);
int dft_plan_r2r(const int dft_points, dft_dir_t dir, dft_plan_t *plan);
int dft_plan_c2r(const int dft_points, dft_dir_t dir, dft_plan_t *plan);

void dft_plan_free(dft_plan_t *plan);
void dft_plan_free_vector(dft_plan_t *plan, int nof_plans);

void dft_run(dft_plan_t *plan, void *in, void *out);
void dft_run_c2c(dft_plan_t *plan, dft_c_t *in, dft_c_t *out);
void dft_run_r2r(dft_plan_t *plan, dft_r_t *in, dft_r_t *out);
void dft_run_c2r(dft_plan_t *plan, dft_c_t *in, dft_r_t *out);

