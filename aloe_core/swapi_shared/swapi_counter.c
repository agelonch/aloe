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

#include "swapi_context.h"
#include "swapi_counter.h"
#include "swapi.h"
#include "nod_waveform.h"


/** \brief Initializes a counter object. This function is called by the swapi_counter_create().
 * It calls nod_module_variable_create() to create a new module variable, which is associated to the
 * counter. Then uses nod_variable_init() to allocate the memory.
 */
int swapi_counter_init(void *context, swapi_counter_t *counter, string name) {
	sdebug("context=0x%x, counter_id=%d, name=%s\n",context,counter->id,name);
	swapi_context_t *ctx = context;
	nod_module_t *module = ctx->module;
	SWAPI_ASSERT_PARAM(counter);
	SWAPI_ASSERT_PARAM(name);
	variable_t *variable = nod_module_variable_create(module, name);
	if (!variable) {
		return -1;
	}
	if (nod_variable_init(variable,sizeof(int))) {
		return -1;
	}
	sdebug("variable_id=%d, addr=0x%x\n",variable->id,variable->cur_value);
	counter->variable = variable;
	counter->context = ctx;
	return 0;
}

/**
 * \brief A counter shall be used by a user to measure time intervals. swapi_counter_create() returns
 * a counter_t object which is passed as a first parameter to swapi_counter_start() and
 * swapi_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the MANAPI interface. The
 * variable value is updated during a call to swapi_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *
 * swapi_counter_close() closes a counter and deallocates its resources. The counter can not be used
 * after a call to this function.
 *
 * \param counter Value returned by the swapi_counter_create() function
 * \returns 0 if successfully closed or -1 on error.
 */
int swapi_counter_close(counter_t counter) {
	swapi_counter_t *cnt = (swapi_counter_t*) counter;
	swapi_context_t *ctx = cnt->context;
	sdebug("context=0x%x, counter_id=%d\n",ctx,cnt->id);
	SWAPI_ASSERT_PARAM(counter);
	if (nod_variable_close(cnt->variable)) {
		return -1;
	}
	cnt->id = 0;
	cnt->variable = NULL;
	return 0;
}

/**
 * \brief A counter shall be used by a user to measure time intervals. swapi_counter_create() returns
 * a counter_t object which is passed as a first parameter to swapi_counter_start() and
 * swapi_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the MANAPI interface. The
 * variable value is updated during a call to swapi_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *  *
 * \param counter Value returned by the swapi_counter_create() function
 * \returns 0 on success or -1 on error.
 */
int swapi_counter_start(counter_t counter) {
	swapi_counter_t *cnt = (swapi_counter_t*) counter;
	swapi_context_t *ctx = cnt->context;
	sdebug("context=0x%x, counter_id=%d, start=%d:%d\n",ctx,cnt->id,
			cnt->count[1].tv_sec,cnt->count[1].tv_usec);
	SWAPI_ASSERT_PARAM(counter);
	hwapi_time_get(&cnt->count[1]);
	return -1;
}

/**
 * \brief A counter shall be used by a user to measure time intervals. swapi_counter_create() returns
 * a counter_t object which is passed as a first parameter to swapi_counter_start() and
 * swapi_counter_stop() functions, which start and stop the counter, respectively. A counter is
 * associated with a public module variable which can be accessed from the MANAPI interface. The
 * variable value is updated during a call to swapi_counter_stop(), saving the elapsed time between
 * successive calls to start() and stop(), in microseconds.
 *  *
 * \param counter Value returned by the swapi_counter_create() function
 * \returns 0 on success or -1 on error.
 */
int swapi_counter_stop(counter_t counter) {
	swapi_counter_t *cnt = (swapi_counter_t*) counter;
	swapi_context_t *ctx = cnt->context;
	SWAPI_ASSERT_PARAM(counter);
	variable_t *variable = cnt->variable;

	hwapi_time_get(&cnt->count[2]);
	hwapi_time_interval(cnt->count);
	sdebug("context=0x%x, counter_id=%d, finish=%d:%d, count=%d, value=0x%x\n",ctx,cnt->id,
			cnt->count[2].tv_sec,cnt->count[2].tv_usec,cnt->count[0].tv_usec,
			variable->cur_value);
	int *value = (int*) variable->cur_value;
	if (!value) {
		return -1;
	}
	*value = cnt->count[0].tv_usec;
	return 0;
}
