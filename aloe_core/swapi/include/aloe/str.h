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

#ifndef STR_H
#define STR_H

#include <string.h>

#define STR_LEN 64
#define LSTR_LEN 512

#define strdef(a) char a[STR_LEN]
#define lstrdef(a) char a[LSTR_LEN]
#define lstring char*
#define string char*

#define strcpy(a,b) strncpy(a,b,STR_LEN)
#define lstrcpy(a,b) strncpy(a,b,LSTR_LEN)
#undef strcmp
#define strcmp(a,b) strncmp(a,b,STR_LEN)
#define lstrcmp(a,b) strncmp(a,b,LSTR_LEN)

#endif
