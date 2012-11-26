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

/* debug hwapi */
#define DEBUG_HWAPI 0
#define hdebug(_fmt, ...) \
	do { if (DEBUG_HWAPI) printf("[debug-hwapi]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug memory */
#define DEBUG_POOL 0
#define memdebug(_fmt, ...) \
	do { if (DEBUG_POOL) printf("[debug-mempool]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);


/* debug packet */
#define DEBUG_PKT 0
#define pktdebug(_fmt, ...) \
	do { if (DEBUG_PKT) printf("[debug-packet]\t[%s()]: pkt=0x%x " _fmt, __func__,pkt,__VA_ARGS__);} while(0);


/* debug serializable */
#define DEBUG_SER 1
#define serdebug(_fmt, ...) \
	do { if (DEBUG_SER) printf("[debug-serial]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug manager */
#define DEBUG_MAN 1
#define mdebug(_fmt, ...) \
	do { if (DEBUG_MAN) printf("[debug-manager]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug parse */
#define DEBUG_PARSER 0
#define pardebug(_fmt, ...) \
	do { if (DEBUG_PARSER) printf("[debug-parser]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug node */
#define DEBUG_NODE 1
#define ndebug(_fmt, ...) \
	do { if (DEBUG_NODE) printf("[debug-node]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);

/* debug swapi */
#define DEBUG_SWAPI 1
#define sdebug(_fmt, ...) \
	do { if (DEBUG_SWAPI) printf("[debug-swapi]\t[%s()]: " _fmt, __func__,__VA_ARGS__);} while(0);


