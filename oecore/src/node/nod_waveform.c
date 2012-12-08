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

#include "defs.h"
#include "packet.h"
#include "hwapi.h"
#include "nod_waveform.h"
#include "mempool.h"
#include "swapi_context.h"



/** \brief Allocates resources for nof_modules in a nod_waveform_t waveform.
 * Does NOT call allocate interfaces/variables for each module.
 */
int nod_waveform_alloc(nod_waveform_t *w, int nof_modules) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, nof_modules);
	aassert(w);
	w->modules = (nod_module_t*) pool_alloc(nof_modules,sizeof(nod_module_t));
	if (!w->modules) return -1;
	w->nof_modules = nof_modules;
	return 0;
}

/** \brief Deallocates nod_waveform_t resources allocated using nod_waveform_alloc().
 * Does NOT call deallocate interfaces/variables for each module.
 */
int nod_waveform_free(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, w->nof_modules);
	aassert(w);
	if (pool_free(w->modules)) return -1;
	w->modules = NULL;
	w->nof_modules = 0;
	return 0;
}

/* \brief nod_waveform_load() calls nod_module_load() for each module in the waveform
 */
int nod_waveform_load(nod_waveform_t *w) {
	time_t t;
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, w->nof_modules);
	aassert(w);
	int i;
	for (i=0;i<w->nof_modules;i++) {
		if (nod_module_load(&w->modules[i])) {
			return -1;
		}
	}
	if (nod_waveform_run(w,1)) {
		ndebug("error running waveform %s. Removing\n",w->name);
		return -1;
	}

	t.tv_sec = 2;
	t.tv_usec = 0;
	hwapi_sleep(&t);
	for (i=0;i<w->nof_modules;i++) {
		if (w->modules[i].parent.status != LOADED) {
			aerror_msg("module_id=%d did not load correctly\n",w->modules[i].parent.id);
			return -1;
		}
	}
	return 0;
}

/* \brief nod_waveform_run() calls nod_module_run() for each module in the waveform
 */
int nod_waveform_run(nod_waveform_t *w, int runnable) {
	ndebug("waveform_id=%d, nof_modules=%d\n",w->id, w->nof_modules);
	aassert(w);
	int i;
	for (i=0;i<w->nof_modules;i++) {
		if (nod_module_run(&w->modules[i], runnable)) {
			return -1;
		}
	}
	w->status.cur_status = LOADED;
	return 0;
}

/* \brief nod_waveform_remove() calls nod_module_remove() for each module in the waveform and
 * sets status = STOP, id = 0 and CALLS nod_waveform_free().
 */
int nod_waveform_remove(nod_waveform_t *w) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d\n",w->id, w->nof_modules,
			w->status.cur_status);
	aassert(w);
	int i;
	for (i=0;i<w->nof_modules;i++) {
		ndebug("removing module_id=%d\n",w->modules[i].parent.id);
		if (w->modules[i].parent.id) {
			if (nod_module_remove(&w->modules[i])) {
				aerror_msg("removing module_id=%d\n",w->modules[i].parent.id);
				return -1;
			}
		}
	}
	if (nod_waveform_free(w)) {
		return -1;
	}
	w->status.cur_status = STOP;
	w->id = 0;
	return 0;
}

static void* nod_waveform_status_stop_thread(void *arg) {
	int i;

	nod_waveform_t* waveform = arg;
	ndebug("waveform_id=%d\n",waveform->id);

	if (DEBUG_NODE) {
		hwapi_task_print_sched();
	}

	for (i=0;i<waveform->nof_modules;i++) {
		if (waveform->modules[i].parent.status != STOP) {
			if (hwapi_process_isrunning(waveform->modules[i].process)) {
				aerror_msg("module %s is still running\n",waveform->modules[i].parent.name);
				return NULL;
			}
			if (nod_module_stop(&waveform->modules[i])) {
				aerror_msg("stopping module %s of waveform %s\n",waveform->modules[i].parent.name,
						waveform->name);
				return NULL;
			}
		}
	}
	return (void*) 1;
}

/** \brief goes through all the modules and calls nod_module_stop();
 */
int nod_waveform_status_stop(nod_waveform_t *waveform) {
	aassert(waveform);
	ndebug("waveform_id=%d\n",waveform->id);
	int i;
	h_task_t task;
	void *ret_val;
	time_t t;
	int n;

	waveform->status.cur_status = STOP;

	if (hwapi_task_new(&task,nod_waveform_status_stop_thread,waveform)) {
		aerror("creating task\n");
		return -1;
	}

	t.tv_sec = 1;
	t.tv_usec = 500000;
	if (hwapi_sleep(&t)) {
		hwapi_error_print("hwapi_sleep");
		return -1;
	}

	n = hwapi_task_wait_nb(task,&ret_val);
	if (n == -1) {
		hwapi_error_print("hwapi_task_wait");
		return -1;
	} else if (n == 0 || (n == 1 && ret_val == NULL)){
		if (n == 0) {
			aerror_msg("Stopping task did not finish (waveform %s)\n", waveform->name);
		}
		aerror_msg("Waveform %s did not stop correctly. Some resources may remain open\n",
					waveform->name);
	} else {
		ainfo("Waveform %s was cleanly removed from the system.\n",waveform->name);
	}

	if (nod_waveform_remove(waveform)) {
		aerror("nod_waveform_remove");
		return -1;
	}

	return 0;

}


void* nod_waveform_status_init_thread(void *arg) {
	int i;
	int n;
	int nof_trials, nof_initiated;
	nod_waveform_t *waveform = arg;

	if (DEBUG_NODE) {
		hwapi_task_print_sched();
	}

	i = nof_trials = nof_initiated = 0;
	while(nof_initiated < waveform->nof_modules) {
		if (waveform->modules[i].parent.status != INIT) {
			n = nod_module_init(&waveform->modules[i]);
			if (n < 0) {
				aerror_msg("initiating module %s\n",waveform->modules[i].parent.name);
				return NULL;
			} else if (n > 0) {
				nof_initiated++;
			}
		}
		i++;
		if (i == waveform->nof_modules) {
			i = 0;
			nof_trials++;
			if (nof_trials == 2*waveform->nof_modules) {
				aerror_msg("Waveform could not initiate after %d trials\n", 2*waveform->nof_modules);
				return NULL;
			}
		}
	}
	return (void*) 1;
}

/** \brief goes through all the modules and calls nod_module_init().
 * Since nod_module_init() may return 0 if the module goes to sleep for one timeslot,
 * we have to pass through all modules several times.
 */
int nod_waveform_status_init(nod_waveform_t *waveform) {
	time_t t;
	int n;
	h_task_t task;
	void *ret_val;

	waveform->status.cur_status = INIT;

	if (hwapi_task_new(&task,nod_waveform_status_init_thread,waveform)) {
		aerror("creating task\n");
		return -1;
	}

	t.tv_sec = 2;
	t.tv_usec = 0;
	if (hwapi_sleep(&t)) {
		hwapi_error_print("hwapi_sleep");
		return -1;
	}

	n = hwapi_task_wait_nb(task,&ret_val);
	if (n == -1) {
		hwapi_error_print("hwapi_task_wait");
		return -1;
	} else if (n == 0) {
		aerror_msg("Init task did not finish (waveform %s)\n", waveform->name);
	} else {
		if (ret_val == NULL) {
			aerror_msg("Waveform %s did not init correctly. Stopping\n",
					waveform->name);
			nod_waveform_status_stop(waveform);
			return -1;
		}
	}

	return 0;
}

/** \brief Changes the status of the waveform to new_status. If the third argument is non-null,
 * it sets the module's process as runnable after setting the waveform status. This is useful for
 * the case when the module has unexpectecly terminated.
 *
 */
int nod_waveform_status_new(nod_waveform_t *waveform, waveform_status_t *new_status) {
	aassert(waveform);
	aassert(new_status);
	int i;
	ndebug("waveform_id=%d, new_status=%d, next_ts=%d\n",waveform->id,
			new_status->cur_status, new_status->next_timeslot);

	if (!new_status->next_timeslot) {
		new_status->next_timeslot = hwapi_time_slot();
	}

	switch(new_status->cur_status) {
	case INIT:
		if (nod_waveform_run(waveform,0)) { /** stop running modules in pipeline */
			return -1;
		}
		if (nod_waveform_status_init(waveform)) {
			return -1;
		}
		break;
	case STOP:
		if (nod_waveform_run(waveform,0)) {
			return -1;
		}
		if (nod_waveform_status_stop(waveform)) {
			return -1;
		}
		break;
	default:
		if (waveform->status.cur_status == STEP) {
			for (i=0;i<waveform->nof_modules;i++) {
				if (waveform->modules[i].parent.status != STEP) {
					aerror_msg("Caution module %s did not step correctly\n",
							waveform->modules[i].parent.name);
				}
				waveform->modules[i].parent.status = PAUSE;
				waveform->modules[i].changing_status = 0;
			}
		}
		nod_waveform_run(waveform,1);/* let modules run in pipeline */
		for (i=0;i<waveform->nof_modules;i++) {
			if (waveform->modules[i].changing_status) {
				aerror_msg("Caution module %s was still changing the status\n",
						waveform->modules[i].parent.name);
				waveform->modules[i].changing_status = 0;
			}
		}
		memcpy(&waveform->status,new_status,sizeof(waveform_status_t));
	}

	return 0;
}


/** \brief Returns a pointer to the first module with id the second parameter
 * \returns a non-null pointer if found or null otherwise
 */
nod_module_t* nod_waveform_find_module_id(nod_waveform_t *w, int module_id) {
	mdebug("waveform=%s, module_id=%d\n",w->name, module_id);
	int i=0;
	while(i<w->nof_modules && w->modules[i].parent.id != module_id)
		i++;
	if (i==w->nof_modules) {
		return NULL;
	}
	return &w->modules[i];
}



/** \brief Serializes a nod_waveform_t object to a packet.
 * \param all_module If non-zero, serializes the entire module structure. If is zero serializes
 * only the execinfo structure
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 */
int nod_waveform_serialize(nod_waveform_t *src, packet_t *pkt, int all_module,
		enum variable_serialize_data copy_data) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d, all_module=%d, copy_data=%d\n",
			src->id, src->nof_modules,src->status.cur_status, all_module, copy_data);
	int i;
	aassert(pkt);
	aassert(src);

	add_i(&all_module);
	add_i(&src->nof_modules);
	ndebug("nof_modules=%d\n",src->nof_modules);
	for (i=0;i<src->nof_modules;i++) {
		add_i(&src->modules[i].parent.id);
		if (all_module) {
			if (module_serialize(&src->modules[i].parent,pkt,copy_data))
				return -1;
		} else {
			if (execinfo_serialize(&src->modules[i].parent.execinfo,pkt))
				return -1;
		}
	}
	return 0;
}

/** \brief Unserializes a nod_waveform_t object from the packet to the structure pointed by dest.
 * If the waveform is in status STOP unserializes all the waveform contents. It unserializes
 * the waveform status only otherwise.
 * The copy_data parameter is obtained from the packet.
 * nod_waveform_unserializeTo() calls nod_waveform_alloc() to allocate memory for the modules
 * and nod_module_alloc() to allocate the swapi context memory for each module.
 */
int nod_waveform_unserializeTo(packet_t *pkt, nod_waveform_t *dest) {
	ndebug("waveform_id=%d, nof_modules=%d, status=%d,\n",
			dest->id, dest->nof_modules,dest->status.cur_status);
	int i;
	int nof_modules, tmp;
	enum variable_serialize_data copy_data;
	aassert(pkt);
	aassert(dest);

	if (dest->status.cur_status == STOP) {
		get_i(&tmp);
		copy_data = (enum variable_serialize_data) tmp;
		get_i(&dest->id);
		if (packet_get_data(pkt,dest->name,STR_LEN)) return -1;

		get_i(&dest->nof_modes);
		for (i=0;i<dest->nof_modes;i++) {
			if (waveform_mode_unserializeTo(pkt,&dest->modes[i])) {
				return -1;
			}
		}

		get_i(&nof_modules);
		ndebug("id=%d, copy_data=%d, nof_modules=%d\n",dest->id,tmp,nof_modules);
		if (nod_waveform_alloc(dest,nof_modules)) return -1;
		for (i=0;i<dest->nof_modules;i++) {
			if (nod_module_alloc(&dest->modules[i])) return -1;
			dest->modules[i].parent.waveform = dest;
			if (module_unserializeTo(pkt,&dest->modules[i].parent, copy_data))
				return -1;
		}
		dest->status.cur_status = LOADED;
	} else {
		waveform_status_t new_status;

		if (packet_get_data(pkt, &new_status, sizeof(waveform_status_t))) {
			return -1;
		}
		if (nod_waveform_status_new(dest,&new_status)) {
			aerror("setting new status\n");
			if (nod_waveform_status_stop(dest)) {
				aerror("stopping waveform\n");
			}
			return -1;
		}
	}
	return 0;
}







