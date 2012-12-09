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

#include "oesr_context.h"
#include "oesr_counter.h"
#include "oesr.h"
#include "nod_waveform.h"


/** \brief Initializes a counter object. This function is called by the oesr_counter_create().
 * It calls nod_module_variable_create() to create a new module variable, which is associated to the
 * counter. Then uses nod_variable_init() to allocate the memory.
 */
int oesr_counter_init(void *context, oesr_counter_t *counter, string name) {
	sdebug("context=0x%x, counter_id=%d, name=%s\n",context,counter->id,name);
	oesr_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	oesr_ASSERT_PARAM(counter);
	oesr_ASSERT_PARAM(name);
	variable_t *variable = nod_module_variable_create(module, name);
	if (!variable) {
		return -1;
	}
	variable->size = sizeof(int);
	variable->cur_value = &counter->count[0].tv_usec;
	sdebug("variable_id=%d, addr=0x%x\n",variable->id,variable->cur_value);
	counter->variable = variable;
	counter->context = ctx;
	return 0;
}

/**
 * \brief A counter shall be used by a user to measure time intervals. oesr_counter_create() returns
 * a counter_t object which is passed as a first parameter to oesr_counter_start() and
 * oesr_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the oesr_man interface. The
 * variable value is updated during a call to oesr_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *
 * oesr_counter_close() closes a counter and deallocates its resources. The counter can not be used
 * after a call to this function.
 *
 * \param counter Value returned by the oesr_counter_create() function
 * \returns 0 if successfully closed or -1 on error.
 */
int oesr_counter_close(counter_t counter) {
	oesr_counter_t *cnt = (oesr_counter_t*) counter;
	oesr_context_t *ctx = cnt->context;
	sdebug("context=0x%x, counter_id=%d\n",ctx,cnt->id);
	oesr_ASSERT_PARAM(counter);
	variable_t *variable = cnt->variable;
	variable->id = 0;
	variable->size = 0;
	cnt->id = 0;
	cnt->variable = NULL;
	return 0;
}

/**
 * \brief A counter shall be used by a user to measure time intervals. oesr_counter_create() returns
 * a counter_t object which is passed as a first parameter to oesr_counter_start() and
 * oesr_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the oesr_man interface. The
 * variable value is updated during a call to oesr_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *  *
 * \param counter Value returned by the oesr_counter_create() function
 * \returns 0 on success or -1 on error.
 */
int oesr_counter_start(counter_t counter) {
	oesr_counter_t *cnt = (oesr_counter_t*) counter;
	oesr_context_t *ctx = cnt->context;
	oesr_ASSERT_PARAM(counter);
	rtdal_time_get(&cnt->count[1]);
	sdebug("context=0x%x, counter_id=%d, start=%d:%d\n",ctx,cnt->id,
			cnt->count[1].tv_sec,cnt->count[1].tv_usec);
	return -1;
}

/**
 * \brief A counter shall be used by a user to measure time intervals. oesr_counter_create() returns
 * a counter_t object which is passed as a first parameter to oesr_counter_start() and
 * oesr_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the oesr_man interface. The
 * variable value is updated during a call to oesr_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *  *
 * \param counter Value returned by the oesr_counter_create() function
 * \returns 0 on success or -1 on error.
 */
int oesr_counter_stop(counter_t counter) {
	oesr_counter_t *cnt = (oesr_counter_t*) counter;
	oesr_context_t *ctx = cnt->context;
	oesr_ASSERT_PARAM(counter);
	variable_t *variable = cnt->variable;

	rtdal_time_get(&cnt->count[2]);
	rtdal_time_interval(cnt->count);
	sdebug("context=0x%x, counter_id=%d, finish=%d:%d, count=%d, value=0x%x\n",ctx,cnt->id,
			cnt->count[2].tv_sec,cnt->count[2].tv_usec,cnt->count[0].tv_usec,
			variable->cur_value);
	return 0;
}

int oesr_counter_usec(counter_t counter) {
	oesr_counter_t *cnt = (oesr_counter_t*) counter;
	oesr_context_t *ctx = cnt->context;
	oesr_ASSERT_PARAM(counter);
	variable_t *variable = cnt->variable;

	return cnt->count[0].tv_usec;
}
