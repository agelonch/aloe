#include "nod_variable.h"

/**
 * Serialize all attributes including variable value (shmPtr) if size>0
 */
void nod_variable_serialize(nod_variable_t *src, packet_t *pkt) {
	aerror("Not yet implemented");
}

/**
 * Save window and period attributes. If size>0, save variable value to shmPtr while checking size.
 * If new window>0 and period>0, add the variable to module.reportingVariable and increase module.nof_report_vars clear the reportPacket and set periodCnt=0
 * If new window=0 and period=0, remove the variable to module.reportingVariable and decrease module.nof_report_vars
 */
void nod_variable_unserializeTo(packet_t *pkt, nod_variable_t *dest) {
	aerror("Not yet implemented");
}
