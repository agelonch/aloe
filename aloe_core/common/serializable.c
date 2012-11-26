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
#include "waveform.h"
#include "serializable.h"
#include "man_platform.h"

/** \brief Serializes an object of type waveform_t into the packet pointed by pkt.
 * If the waveform is in status PARSED or LOADED serializes all the waveform contents. It serializes
 * the waveform status only otherwise.
 * \param loading_node_id Non-negative integer indicating the node id to which send the modules. Only
 * those modules such that module->node->id=loading_node_id will be serialized.
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 * \returns 0 on success -1 on error
 */
int waveform_serialize(waveform_t *src, packet_t *pkt, int loading_node_id,
		enum variable_serialize_data copy_data) {
	serdebug("src=0x%x, pkt=0x%x, loading_node_id=%d, copy_data=%d, wave_id=%d, "
			"waveform_status=%d\n",src,pkt, loading_node_id,copy_data,
			src->status.cur_status, src->id, src->status.cur_status);
	int i, nof_modules, tmp;
	aassert(src);
	aassert(pkt);

	if (src->status.cur_status == PARSED) {
		tmp = (int) copy_data;
		add_i(&tmp);
		add_i(&src->id);
		if (packet_add_data(pkt,src->name,STR_LEN)) return -1;

		add_i(&src->nof_modes);
		for (i=0;i<src->nof_modes;i++) {
			if (waveform_mode_serialize(&src->modes[i],pkt)) {
				return -1;
			}
		}

		nof_modules = 0;
		for (i=0;i<src->nof_modules;i++) {
			man_node_t *node = src->modules[i].node;
			if (loading_node_id == node->id) {
				nof_modules++;
			}
		}
		hdebug("nof_modules=%d\n",nof_modules);
		add_i(&nof_modules);
		for (i=0;i<src->nof_modules;i++) {
			man_node_t *node = src->modules[i].node;
			if (loading_node_id == node->id) {
				hdebug("serialize module %d\n",i);
				if (module_serialize(&src->modules[i],pkt,copy_data))
					return -1;
			}
		}
	} else {
		if (packet_add_data(pkt, &src->status,sizeof(waveform_status_t)))
			return -1;
	}
	return 0;
}

/** \brief Unserializes an object of type waveform_t to the pointer dest from the packet pointed by pkt.
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 * \returns 0 on success -1 on error
 */
int waveform_unserializeTo(packet_t *pkt, waveform_t *dest,
		enum variable_serialize_data copy_data) {
	serdebug("dest=0x%x, pkt=0x%x, copy_data=%d, wave_id=%d, "
				"waveform_status=%d\n",dest,pkt, copy_data,
				dest->status.cur_status, dest->id, dest->status.cur_status);
	int i,j;
	int module_id;
	int nof_modules;
	int all_module;

	aassert(dest);
	aassert(pkt);
	get_i(&all_module);
	get_i(&nof_modules);
	hdebug("nof_modules=%d, all_module=%d\n",nof_modules, all_module);
	if (waveform_alloc(dest,nof_modules)) return -1;
	for (j=0;j<nof_modules;j++) {
		get_i(&module_id);
		i=0;
		while(i<dest->nof_modules && dest->modules[i].id != module_id)
			i++;
		if (i==dest->nof_modules) {
			return -1;
		}
		dest->modules[i].waveform = dest;
		hdebug("unserialize module %d\n",i);
		if (all_module) {
			if (module_unserializeTo(pkt,&dest->modules[i],copy_data))
				return -1;
		} else {
			if (execinfo_unserializeTo(pkt,&dest->modules[i].execinfo))
				return -1;
		}
	}
	return 0;
}

int waveform_mode_serialize(waveform_mode_t *src, packet_t *pkt) {
	hdebug("src=0x%x, pkt=0x%x\n",src,pkt);
	if (packet_add_data(pkt, src, sizeof(waveform_mode_t))) {
		return -1;
	}
	return 0;
}


int waveform_mode_unserializeTo(packet_t *pkt, waveform_mode_t *dest) {
	hdebug("dest=0x%x, pkt=0x%x\n",dest,pkt);
	if (packet_get_data(pkt, dest, sizeof(waveform_mode_t))) {
		return -1;
	}
	return 0;
}

/** \brief Serializes an object of type module_t into the packet pointed by pkt.
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 * \returns 0 on success -1 on error
 */
int module_serialize(module_t *src, packet_t *pkt, enum variable_serialize_data copy_data) {
	int i;
	serdebug("src=0x%x, pkt=0x%x, copy_data=%d, module_id=%d\n",src,pkt, copy_data, src->id);

	aassert(src);
	aassert(pkt);
	add_i(&src->id);
	add_i(&src->processor_idx);
	add_i(&src->exec_position);
	add_i(&src->nof_modes);
	add_i(&src->cur_mode);
	if (packet_add_data(pkt,src->name,STR_LEN)) return -1;
	if (packet_add_data(pkt,src->binary,STR_LEN)) return -1;
	if (execinfo_serialize(&src->execinfo,pkt)) return -1;
	add_i(&src->nof_variables);
	add_i(&src->nof_inputs);
	add_i(&src->nof_outputs);
	hdebug("nof_inputs=%d, nof_outputs=%d, nof_variables=%d\n",src->nof_inputs,src->nof_outputs,
			src->nof_variables);
	for (i=0;i<src->nof_variables;i++) {
		if (variable_serialize(&src->variables[i],pkt,copy_data,src->nof_modes)) return -1;
	}
	for (i=0;i<src->nof_inputs;i++) {
		if (interface_serialize(&src->inputs[i],pkt)) return -1;
	}
	for (i=0;i<src->nof_outputs;i++) {
		if (interface_serialize(&src->outputs[i],pkt)) return -1;
	}
	return 0;
}
/** \brief Unserializes an object of type module_t to the pointer dest from the packet pointed by pkt.
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 * \returns 0 on success -1 on error
 */
int module_unserializeTo(packet_t *pkt, module_t *dest, enum variable_serialize_data copy_data) {
	int i,nof_variables,nof_inputs,nof_outputs;
	serdebug("dest=0x%x, pkt=0x%x, copy_data=%d, module_id=%d\n",dest,pkt, copy_data);

	aassert(dest);
	aassert(pkt);
	get_i(&dest->id);
	get_i(&dest->processor_idx);
	get_i(&dest->exec_position);
	get_i(&dest->nof_modes);
	get_i(&dest->cur_mode);
	if (packet_get_data(pkt,dest->name,STR_LEN)) return -1;
	if (packet_get_data(pkt,dest->binary,STR_LEN)) return -1;
	if (execinfo_unserializeTo(pkt,&dest->execinfo)) return -1;
	get_i(&nof_variables);
	get_i(&nof_inputs);
	get_i(&nof_outputs);
	hdebug("module_id=%d, nof_inputs=%d, nof_outputs=%d, nof_variables=%d\n",dest->id,nof_inputs,
			nof_outputs, nof_variables);
	if (module_alloc(dest,nof_inputs,nof_outputs,nof_variables)) {
		return -1;
	}
	for (i=0;i<dest->nof_variables;i++) {
		if (variable_alloc(&dest->variables[i],dest->nof_modes)) return -1;
		if (variable_unserializeTo(pkt,&dest->variables[i],copy_data,dest->nof_modes)) return -1;
	}
	for (i=0;i<dest->nof_inputs;i++) {
		if (interface_unserializeTo(pkt,&dest->inputs[i])) return -1;
	}
	for (i=0;i<dest->nof_outputs;i++) {
		if (interface_unserializeTo(pkt,&dest->outputs[i])) return -1;
	}
	return 0;
}

/** \brief Serializes an object of type variable_t into the packet pointed by pkt.
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 * \returns 0 on success -1 on error
 * \param nof_modes Indicates the number of configuration modes that will be copied to the packet
 */
int variable_serialize(variable_t *src, packet_t *pkt, enum variable_serialize_data copy_data,
		int nof_modes) {
	serdebug("src=0x%x, pkt=0x%x, copy_data=%d, nof_modes=%d var_id=%d, size=%d\n",src,pkt, copy_data,
			nof_modes, src->id, src->size);

	int i;
	aassert(src);
	aassert(pkt);

	add_i(&src->id);
	add_i(&src->size);

	switch(copy_data) {
	case CP_INIT:
		if (packet_add_data(pkt,src->name,STR_LEN)) return -1;
		for (i=0;i<nof_modes;i++) {
			if (packet_add_data(pkt,src->init_value[i],src->size)) return -1;
		}
		break;
	case CP_VALUE:
		if (packet_add_data(pkt,src->cur_value,src->size)) return -1;
		break;
	case NONE:
		add_i(&src->window);
		add_i(&src->period);
		break;
	}
	return 0;
}
/** \brief Unerializes an object of type variable_t to the buffer dest from the packet pointed by pkt.
 * \param copy_data Indicates what kind of data will be send for each variable. See enum variable_serialize_data
 * \returns 0 on success -1 on error
 * \param nof_modes Indicates the number of configuration modes that will be copied from the packet
 */
int variable_unserializeTo(packet_t *pkt, variable_t *dest,
		enum variable_serialize_data copy_data, int nof_modes) {
	serdebug("dest=0x%x, pkt=0x%x, copy_data=%d, nof_modes=%d\n",dest,pkt, copy_data, nof_modes);

	int i;
	aassert(dest);
	aassert(pkt);

	get_i(&dest->id);
	get_i(&dest->size);
	hdebug("var_id=%d, size=%d\n",dest->id,dest->size);
	switch(copy_data) {
	case CP_INIT:
		if (packet_get_data(pkt,dest->name,STR_LEN)) return -1;
		for (i=0;i<nof_modes;i++) {
			if (packet_get_data(pkt,dest->init_value[i],dest->size)) return -1;
		}
		break;
	case CP_VALUE:
		if (packet_get_data(pkt,dest->cur_value,dest->size)) return -1;
		break;
	case NONE:
		get_i(&dest->window);
		get_i(&dest->period);
		break;
	}
	return 0;
}

int execinfo_serialize(execinfo_t *src, packet_t *pkt) {
	hdebug("src=0x%x, pkt=0x%x\n",src,pkt)
	aassert(src);
	aassert(pkt);
	if (packet_add_data(pkt, src, sizeof(execinfo_t))) {
		return -1;
	}
	return 0;
}

int execinfo_unserializeTo(packet_t *pkt, execinfo_t *dest) {
	hdebug("dest=0x%x, pkt=0x%x\n",dest,pkt)

	aassert(dest);
	aassert(pkt);
	if (packet_get_data(pkt, dest, sizeof(execinfo_t))) {
		return -1;
	}
	return 0;
}


int interface_serialize(interface_t *src, packet_t *pkt) {
	hdebug("src=0x%x, pkt=0x%x\n",src,pkt)
	aassert(src);
	aassert(pkt);
	if (packet_add_data(pkt, src, sizeof(interface_t))) {
		return -1;
	}
	return 0;
}

int interface_unserializeTo(packet_t *pkt, interface_t *dest) {
	hdebug("dest=0x%x, pkt=0x%x\n",dest,pkt)
	aassert(dest);
	aassert(pkt);
	if (packet_get_data(pkt, dest, sizeof(interface_t))) {
		return -1;
	}
	return 0;
}
