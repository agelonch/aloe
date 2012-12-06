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
#include "man_mapping.h"
#include "man_platform.h"
#include "nod_waveform.h"
#include "mempool.h"

static int mapping_alloc(mapping_t *m, int nof_modules) {
	mdebug("addr=0x%x, nof_modules=%d\n",m,nof_modules);
	if (m->p_res) return -1;
	m->p_res = (int*) pool_alloc(nof_modules,sizeof(int));
	if (!m->p_res) return -1;
	memset(m->modules_x_node,0,sizeof(int)*MAX(nodes));
	return 0;
}

static int mapping_free(mapping_t *m) {
	mdebug("addr=0x%x\n",m);
	if (m->p_res) {
		if (pool_free(m->p_res)) {
			return -1;
		}
		m->p_res = NULL;
	}
	return 0;
}

/** USES MANAPI_ERROR to describe any mapping error */
int setup_algorithm(mapping_t *m) {
	return 0;
}

/** USES MANAPI_ERROR to describe any mapping error */
int generate_model(mapping_t *m) {
	return 0;
}

/** USES MANAPI_ERROR to describe any mapping error */
int call_algorithm(mapping_t *m, waveform_t *waveform, man_platform_t *platform) {
	int i;
	for (i=0;i<waveform->nof_modules;i++) {
		m->p_res[i]=i%platform->nof_processors;
	}
	return 0;
}

/** \brief Maps waveform to the platform. The platform is obtained using the man_platform_get_context()
 * function.
 * The mapping result is saved to each module in the module_t.processor_idx and module_t.exec_position
 * fields. module_t.node is pointed to the platform node object of the allocated processor.
 * The number of nodes allocated to each node is saved in the mapping_t.modules_x_node array.
 * \returns 0 if the mapping was feasible, -1 otherwise. The error description is saved in the
 * manapi_error class.
 */
int mapping_map(mapping_t *m, waveform_t *waveform) {
	/**@TODO: Use the manapi_error class for error messages */

	mdebug("waveform_name=%s, nof_modules=%d\n",waveform->name, waveform->nof_modules);
	int i;
	int ret=-1;
	man_platform_t *platform = man_platform_get_context();
	man_node_t *node;
	if (!platform) {
		aerror("MANAPI not initialized\n");
		return -1;
	}
	if (mapping_alloc(m,waveform->nof_modules)) {
		return -1;
	}
	if (setup_algorithm(m)) {
		goto free;
	}
	if (generate_model(m)) {
		goto free;
	}
	if (call_algorithm(m,waveform, platform)) {
		goto free;
	}
	memset(m->modules_x_node,0,sizeof(int)*MAX(nodes));
	for (i=0;i<waveform->nof_modules;i++) {
		if (m->p_res[i] >= platform->nof_processors) {
			aerror_msg("Module %d mapped to processor %d, but platform has %d processors only\n",
					i,m->p_res[i]+1,platform->nof_processors);
			goto free;
		}
		man_processor_t *p = (man_processor_t*) platform->processors[m->p_res[i]];
		waveform->modules[i].node = p->node;
		waveform->modules[i].processor_idx = p->idx_in_node;
		waveform->modules[i].exec_position = waveform->nof_modules-i-1;
		node = p->node,
		m->modules_x_node[node->id]++;
	}
	ret = 0;
free:
	mapping_free(m);
	return ret;
}
