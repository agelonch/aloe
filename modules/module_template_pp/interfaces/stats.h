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


/* Configure here variables you want to make public.
 */
struct utils_variables vars_extra[] = {

		/*
		{"another_variable",
			STAT_TYPE_INT,
			1,
			&another_variable,
			WRITE},
		*/

		/* In case of visualizing signals, set the variable address to NULL and use the
		 * signal name, e.g. output_0, output_1, input_0, etc.*/
		{"output_0",
			STAT_TYPE_FLOAT,
			512,
			NULL,
			WRITE},



		{NULL, 0, 0, 0, 0}};

const int nof_extra_vars = 1;
