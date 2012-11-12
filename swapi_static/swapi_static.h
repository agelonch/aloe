#ifndef SWAPI_STATIC_H
#define SWAPI_STATIC_H

/**
 * This class has to be linked statically with any ALOE waveform module.
 */

/**
 * This is an internal SWAPI function, called by the hwapi_processs each timeslot. It does not follow the public method naming convention.
 * This function manages the status state machine, computes execution time statistics, writes log files and produces variable reports. The status STOP and INIT are executed only once in non real-time priority.
 */
int _run_cycle(void* context);

/**
 * called from the kernel or the swapi due to some aerror. Set mymodule.status=STOP and call sStop()
 */
void _abort(void* context);

int Run(void *context);

#endif
