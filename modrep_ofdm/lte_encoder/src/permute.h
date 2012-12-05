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

#define TURBO_RATE 3

#define MAX_ROWS 20
#define MAX_COLS 256

#define SIZE_U 10240 	//tamany maxim en bytes que pot ocupar U
#define SIZE_PER 20456  //tamany en bytes del vector permutacions (5114*4bytes)

typedef int Tper; // tipus array de permutacions (TurboCodi)

struct permute_t {
	Tper *PER; 		// array de permutacions
	Tper *DESPER;	// array de despermutacions
};

int mcd(int x,int y);
int ComputePermutation(struct permute_t *permute, int Long_CodeBlock);
