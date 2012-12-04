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

#ifndef SKELETON_H
#define SKELETON_H

#include <assert.h>
#include <stdio.h>

#define USE_LOG 0
#define MOD_DEBUG 0


/**\brief Returns the number of samples (of size input_sample_sz) received from the input port
 * idx.
 * \returns non-negative integer on success or -1 if idx is not a valid interface.
 */
int get_input_samples(int idx);

/**\brief Sets the number of samples (of size output_sample_sz) to send throught the output port
 * idx.
 * \returns 0 on success or -1 on error.
 */
int set_output_samples(int idx, int len);

#ifdef _ALOE_OLD_SKELETON
void* param_get_addr(char *name);
#endif



#ifdef _COMPILE_MEX
#include "mex.h"
#endif


#ifdef _ALOE_OLD_SKELETON
int initialize();
int stop();
#define in(ptr,idx) &ptr[idx*INPUT_MAX_DATA]
#define out(ptr,idx) &ptr[idx*OUTPUT_MAX_DATA]
int get_input_max_samples();
int get_output_max_samples();
#else
int work(void **input, void **output);
int initialize();
int stop();
int generate_input_signal(void *input, int *input_length);
#endif


#ifdef _COMPILE_ALOE
extern void *ctx;
extern log_t mlog;
#define INTERFACE_CONFIG
#endif

/* Info and error messages print */
#define INFOSTR "[info at "
#define ERRSTR "[error at "

#ifdef _COMPILE_ALOE
#define WHERESTR  "%s, line %d]: "
#define WHEREARG  swapi_module_name(ctx), __LINE__
#else
#define WHERESTR  "file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#endif

#define DEBUGPRINT2(out,...)	fprintf(out,__VA_ARGS__)

#ifdef _COMPILE_MEX
#undef DEBUGPRINT2
#define DEBUGPRINT2(out,...)       mexPrintf(__VA_ARGS__)
#endif

#ifdef _COMPILE_ALOE
#undef DEBUGPRINT2
#define DEBUGPRINT2(out,...)       do {if (mlog && USE_LOG) { swapi_log_printf(mlog,__VA_ARGS__); }\
					else {fprintf(out, __VA_ARGS__); } } while(0)
#endif


#define aerror_msg(_fmt, ...)  DEBUGPRINT2(stderr,ERRSTR WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#define aerror(a)  DEBUGPRINT2(stderr, ERRSTR WHERESTR a, WHEREARG)

#define ainfo(a) DEBUGPRINT2(stdout, INFOSTR WHERESTR a, WHEREARG)
#define ainfo_msg(_fmt, ...)  DEBUGPRINT2(stdout,INFOSTR WHERESTR _fmt, WHEREARG, __VA_ARGS__)

#define modinfo 		ainfo
#define modinfo_msg 	ainfo_msg
#define moderror 		aerror
#define moderror_msg 	aerror_msg

#ifdef _COMPILE_ALOE
#define moddebug(_fmt, ...) \
	do { if (MOD_DEBUG) printf("[mod_debug-%s]\t[%s()]: " _fmt, swapi_module_name(ctx),__func__,\
			__VA_ARGS__);} while(0);
#else
#define moddebug(_fmt, ...) \
	do { if (MOD_DEBUG) printf("[mod_debug]\t[%s()]: " _fmt, __func__,\
			__VA_ARGS__);} while(0);
#endif

#endif

