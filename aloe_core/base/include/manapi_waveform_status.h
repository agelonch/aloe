#ifndef MANAPI_WAVEFORM_STATUS_H_
#define MANAPI_WAVEFORM_STATUS_H_

typedef enum {
	PARSED, LOADED, INIT, RUN, PAUSE, STOP
} waveform_status_enum;

typedef struct {
	waveform_status_enum cur_status;
	int next_timeslot;
	/**
	 * Indicates the deadline timeslot to check if the status has been changed correctly.
	 */
	int dead_timeslot;
} waveform_status_t;



#endif
