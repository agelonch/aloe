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

#include "permute.h"

#define NUMREGS 3

struct permute_t permute;


void turbo_coder(char *input, char *output, int long_cb) {
	
	char reg1_0,reg1_1,reg1_2, reg2_0,reg2_1,reg2_2;
	int i,k=0,j;
	char bit;
	char in,out;
	Tper *per;
	int n;

	n=ComputePermutation(&permute, long_cb);

	per=permute.PER;
	
	reg1_0=0;
	reg1_1=0;
	reg1_2=0;
	
	reg2_0=0;
	reg2_1=0;
	reg2_2=0;
	
	k=0;
	for (i=0;i<long_cb;i++) {
		bit=input[i];
		
		output[k]=bit;
		k++;
		
		in=bit^(reg1_2^reg1_1);
		out=reg1_2^(reg1_0^in);

		reg1_2=reg1_1;
		reg1_1=reg1_0;
		reg1_0=in;
		
		output[k]=out;
		k++;
		
		bit=input[per[i]];
		
		in=bit^(reg2_2^reg2_1);
		out=reg2_2^(reg2_0^in);

		reg2_2=reg2_1;
		reg2_1=reg2_0;
		reg2_0=in;
		
		output[k]=out;
		k++;
	}
	
	k=3*long_cb;
	
	/* TAILING DEL CODER #1 */
	for (j=0;j<NUMREGS;j++) {
		bit=reg1_2^reg1_1;
		
		output[k]=bit;
		k++;
		
		in=bit^(reg1_2^reg1_1);
		out=reg1_2^(reg1_0^in);

		reg1_2=reg1_1;
		reg1_1=reg1_0;
		reg1_0=in;
		
		output[k]=out;
		k++;
	}
	/* TAILING DEL CODER #2 */
	
	for (j=0;j<NUMREGS;j++) {
		bit=reg2_2^reg2_1;
		
		output[k]=bit;
		k++;
			
		in=bit^(reg2_2^reg2_1);
		out=reg2_2^(reg2_0^in);

		reg2_2=reg2_1;
		reg2_1=reg2_0;
		reg2_0=in;
		
		output[k]=out;
		k++;
	}	
}

