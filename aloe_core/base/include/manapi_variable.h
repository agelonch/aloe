#ifndef MANAPI_VARIABLE_H_
#define MANAPI_VARIABLE_H_

/**
 * Inherits from General.ModVariable. Part of the CMDAPI. Adds functionality to set or obtain variable values from the distributed modules.
 */
typedef struct {
	/**
	 * Unique id in the waveform
	 */
	int id;
	/**
	 * Size of the variable, in bytes
	 */
	int size;
	/**
	 * Name of the variable, as it is created by the module or defined in the .app. Note the module must create its variable with the same name as defined in the .app if wants to use the initialization value.
	 */
	strdef(name);

	/**
	 * Number of samples to capture from the variable during variable reporting. If the variable has len bytes, window*len bytes will be captured each period timeslots.
	 */
	int window;
	/**
	 * Period, in timeslots, the variable value is reported.
	 */
	int period;
	int periodCnt;
	void* cur_value;

} variable_t;


#endif
