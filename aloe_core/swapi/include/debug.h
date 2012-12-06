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


//#define DEBUG_TRACE

#ifdef DEBUG_TRACE
#ifndef _DEBUG_TRACE
#define _DEBUG_TRACE
extern FILE *trace_buffer;
#endif
#define debug_buffer trace_buffer
#else
#define debug_buffer stdout
#endif

/* debug hwapi */
#define DEBUG_HWAPI 0
#define hdebug(_fmt, ...) \
	do { if (DEBUG_HWAPI) fprintf(debug_buffer,"[debug-hwapi]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug memory */
#define DEBUG_POOL 0
#define memdebug(_fmt, ...) \
	do { if (DEBUG_POOL) fprintf(debug_buffer,"[debug-mempool]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);


/* debug packet */
#define DEBUG_PKT 0
#define pktdebug(_fmt, ...) \
	do { if (DEBUG_PKT) fprintf(debug_buffer,"[debug-packet]\t[%s()]: pkt=0x%x " _fmt, __func__,pkt,__VA_ARGS__);} while(0);


/* debug serializable */
#define DEBUG_SER 0
#define serdebug(_fmt, ...) \
	do { if (DEBUG_SER) fprintf(debug_buffer,"[debug-serial]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug manager */
#define DEBUG_MAN 0
#define mdebug(_fmt, ...) \
	do { if (DEBUG_MAN) fprintf(debug_buffer,"[debug-manager]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug parse */
#define DEBUG_PARSER 0
#define pardebug(_fmt, ...) \
	do { if (DEBUG_PARSER) fprintf(debug_buffer,"[debug-parser]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug node */
#define DEBUG_NODE 0
#define ndebug(_fmt, ...) \
	do { if (DEBUG_NODE) fprintf(debug_buffer,"[debug-node]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug swapi */
#define DEBUG_SWAPI 0
#define sdebug(_fmt, ...) \
	do { if (DEBUG_SWAPI) fprintf(debug_buffer,"[debug-swapi]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);


