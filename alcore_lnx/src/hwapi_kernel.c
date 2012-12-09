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

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "hwapi_kernel.h"
#include "str.h"
#include "defs.h"
#include "hwapi_time.h"
#include "hwapi_process.h"
#include "hwapi_context.h"
#include "hwapi_task.h"
#include "hwapi.h"
#include "hwapi_timer.h"
#include "futex.h"

#define KERNEL_SIG_THREAD_SPECIFIC SIGRTMIN
#define N_THREAD_SPECIFIC_SIGNALS 6
const int thread_specific_signals[N_THREAD_SPECIFIC_SIGNALS] =
	{SIGSEGV, SIGBUS, SIGILL, SIGFPE, TASK_TERMINATION_SIGNAL, SIGUSR1};


int *core_mapping;
int nof_cores;

static hwapi_context_t hwapi;
static hwapi_timer_t kernel_timer;
static long int timeslot_us;
static enum clock_source clock_source;

static int sigwait_stops = 0;
static int multi_timer_futex;
static pid_t kernel_pid;
static pthread_t single_timer_thread;
static char UNUSED(sigmsg[1024]);
static int signal_received = 0;

static void go_out();
static void thread_signal_handler(int signum, siginfo_t *info, void *ctx);
static void print_license();

FILE *trace_buffer = NULL;
char *debug_trace_addr;
size_t debug_trace_sz;
FILE *debug_trace_file;



/**
 * A real-time fault has been detected. if hwapi.machine.rtFaultKill, calls
 * procThread.restoreThread() to kill the runningModule and restore the execution.
 * The process causing the real-time failure is removed from the ProcThread object
 * and a new thread is created beginning the execution with the first element in the pipeline.
 *
 * Set aerrorCode=RTFAULT value in module's Process object.
 *
 * *NOTE* that meanwhile, the module may be already finished and the next module
 *  in the pipeline will still receive an RT-fault. Indeed, when there is an
 *  rt-fault it is a system-wide problem, despite identifying which module is
 *  causing it is always helpful.
 */


inline static void kernel_tslot_run_rt_control() {
	if (hwapi.machine.rt_fault_opts == RT_FAULT_OPTS_HARD) {
		for (int i=0;i<hwapi.machine.nof_cores;i++) {
			hdebug("tslot=%d, pipeline=%d, ts_counter=%d, finished=%d\n",hwapi_time_slot(),
					hwapi.pipelines[i].id,hwapi.pipelines[i].ts_counter, hwapi.pipelines[i].finished);
			if (!hwapi.pipelines[i].finished
					&& hwapi.pipelines[i].running_process
					&& hwapi.pipelines[i].ts_counter < hwapi_time_slot()-1) {
				if (pipeline_rt_fault(&hwapi.pipelines[i])) {
					aerror("Couldn't kill pipeline after an rt-fault, "
							"going out\n");
				}
			}
		}
	}  else {
		aerror("Not implemented\n");
	}
}

static inline void kernel_tslot_run_periodic_callbacks() {
	/* Call periodic functions */
	for (int i=0;i<hwapi.nof_periodic;i++) {
		hdebug("function %d, counter %d\n",i,hwapi.periodic[i].counter);
		if (hwapi.periodic[i].counter==hwapi.periodic[i].period)  {
			hdebug("function %d, calling\n",i);
			hwapi.periodic[i].callback();
			hwapi.periodic[i].counter=0;
		}
		hwapi.periodic[i].counter++;
	}
}

inline void kernel_tslot_run() {
	hwapi_time_ts_inc();

	hdebug("tslot=%d\n",hwapi_time_slot());

	if (signal_received) {
		signal_received = 0;
	}

	kernel_tslot_run_rt_control();

	kernel_tslot_run_periodic_callbacks();
}

static int first_cycle = 0;
/**
 * This function is called by the internal timer, a DAC event or by the sync_slave,
 * after the reception of a synchronization packet.
 */
static void kernel_cycle(void *x, struct timespec *time) {
	hdebug("now is %d:%d\n",time->tv_sec,time->tv_nsec);
	if (!first_cycle) {
		hwapi_time_reset_realtime(time);
		first_cycle = 1;
	}
	kernel_tslot_run();
	pipeline_sync_threads();
}

inline static int kernel_initialize_setup_clock() {
	struct timespec start_time;

	/* access to kernel sync function is not allowed */
	hwapi.machine.slave_sync_kernel = NULL;

	switch(hwapi.machine.clock_source) {
	case  SINGLE_TIMER:
		kernel_timer.period_function = kernel_cycle;
		kernel_timer.period = hwapi.machine.ts_len_us*1000;
		kernel_timer.arg = NULL;
		kernel_timer.mode = NANOSLEEP;
		kernel_timer.wait_futex = NULL;
		kernel_timer.thread = &single_timer_thread;
		hdebug("creating single_timer_thread period %d\n",(int) kernel_timer.period);
		if (hwapi_task_new_thread(&single_timer_thread, timer_run_thread,
				&kernel_timer, DETACHABLE,
				hwapi.machine.kernel_prio-1, 0,0)) {
			hwapi_perror("hwapi_task_new_thread");
			return -1;
		}
		break;
	case MULTI_TIMER:
		usleep(100000);
		printf("Starting clocks in %d sec...\n", TIMER_FUTEX_GUARD_SEC);
		clock_gettime(CLOCK_MONOTONIC, &start_time);
		for (int i=0;i<hwapi.machine.nof_cores;i++) {
			hwapi.pipelines[i].mytimer.next = start_time;
		}
		futex_wake(&multi_timer_futex);
		break;
	case DAC:
	case SYNC_SLAVE:
		/* enable access to kernel_cycle function */
		hwapi.machine.slave_sync_kernel = kernel_cycle;
		break;
	default:
		aerror_msg("Unknown clock source %d\n", hwapi.machine.clock_source);
		return -1;
	}
	return 0;
}

int kernel_initialize_create_pipeline(pipeline_t *obj, int *wait_futex) {
	void *(*tmp_thread_fnc)(void*);
	void *tmp_thread_arg;

	hdebug("pipeline_id=%d\n",obj->id);
	if (hwapi.machine.clock_source == MULTI_TIMER) {
		obj->mytimer.period_function =
				pipeline_run_from_timer;
		obj->mytimer.period =
				hwapi.machine.ts_len_us*1000;
		obj->mytimer.arg = obj;
		obj->mytimer.wait_futex = wait_futex;
		obj->mytimer.mode = NANOSLEEP;

		obj->mytimer.thread =
				&obj->thread;
		tmp_thread_fnc = timer_run_thread;
		tmp_thread_arg = &obj->mytimer;
	} else {
		tmp_thread_fnc = pipeline_run_thread;
		tmp_thread_arg = obj;
	}

	if (hwapi_task_new_thread(&obj->thread,
			tmp_thread_fnc, tmp_thread_arg, DETACHABLE,
			hwapi.machine.kernel_prio-2, core_mapping[obj->id],0)) {
		hwapi_perror();
		return -1;
	}
	return 0;
}

/**
 * Creates the pipelines.
 * @return
 */
inline static int kernel_initialize_create_pipelines() {


	pipeline_initialize(hwapi.machine.nof_cores);
	hdebug("creating %d pipeline threads\n",hwapi.machine.nof_cores);

	for (int i=0;i<hwapi.machine.nof_cores;i++) {
		hwapi.pipelines[i].id = i;
		if (kernel_initialize_create_pipeline(&hwapi.pipelines[i],&multi_timer_futex)) {
			aerror("Creating pipeline\n");
			return -1;
		}
	}

	return 0;
}

/**
 * Block all signals except (SIGSEGV,SIGILL,SIGFPE,SIGBUS),
 * which sigwaited by the sigwait_thread
 */
inline static int kernel_initialize_setup_signals() {
	int s;
	sigset_t set;
	int i;
	struct sigaction action;

	action.sa_sigaction = thread_signal_handler;
	action.sa_flags = SA_SIGINFO;

        sigfillset(&set);
        /* do not block thread-specific signals. Add a handler to them */
        for (i=0;i<N_THREAD_SPECIFIC_SIGNALS;i++) {
                sigdelset(&set, thread_specific_signals[i]);
                if (sigaction(thread_specific_signals[i], &action, NULL)) {
                	poserror(errno, "sigaction");
                	return -1;
                }
        }
        s = sigprocmask(SIG_BLOCK, &set, NULL);
	if (s != 0) {
		poserror(errno, "sigprocmask");
		return -1;
	}
	return 0;
}

inline static int kernel_initialize_set_kernel_priority() {
	struct sched_param param;
	cpu_set_t cpuset;

	param.sched_priority = hwapi.machine.kernel_prio;
	hdebug("kernel_prio=%d\n",hwapi.machine.kernel_prio);

	if (sched_setscheduler(0, SCHED_FIFO, &param)) {
		if (errno == EPERM) {
			awarn("Not enough privileges to set kernel thread priority\n");
		} else {
			poserror(errno, "sched_setscheduler");
			return -1;
		}
	}

	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);

	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset)) {
		poserror(errno, "sched_setaffinity");
		return -1;
	}

	return 0;
}

/**
 * Initializes the entire hwapi and kernel. This compromises:
 * a) initializing hwapi_base (parse platform file, create interfaces, etc.)
 * b) initialize node (configure and prepare interfaces, create processing
 * 	messages thread, etc.)
 * c) create a shared memory area for time sharing with the manager
 * 	(may be another process in the same processor)
 * c) setup signals
 * d) create different threads and assign priorities
 */
static int kernel_initialize(void) {

	/* Initialize hwapi_base library */
	hwapi.machine.clock_source = clock_source;
	hwapi.machine.ts_len_us = timeslot_us;
	hwapi.machine.kernel_prio = 50;
	hwapi.machine.rt_fault_opts = RT_FAULT_OPTS_HARD;
	hwapi_initialize_node(&hwapi, NULL, NULL);

	pthread_mutex_init(&hwapi.mutex,NULL);

	/* Set self priority to hwapi.machine.kernel_prio */
	if (kernel_initialize_set_kernel_priority()) {
		return -1;
	}

	/* Setup signals */
	if (kernel_initialize_setup_signals()) {
		return -1;
	}

	/* create pipelines */
	if (kernel_initialize_create_pipelines()) {
		return -1;
	}

	/* setup clock */
	if (kernel_initialize_setup_clock()) {
		return -1;
	}

	return 0;
}


/**
 * called after kernel timer caught a synchronous signal.
 * todo: try to recover thread?
 */
static void kernel_timer_recover_thread() {
	aerror("Not implemented\n");
}

static int sigwait_loop_process_thread_signal(siginfo_t *info) {
	int thread_id, signum;
	strdef(tmp_msg);

	signum = info->si_value.sival_int & 0xffff;
	thread_id = info->si_value.sival_int >> 16;
	hdebug("signum=%d, thread_id=%d\n",signum,thread_id);
	if (signum < N_THREAD_SPECIFIC_SIGNALS) {
		sprintf(tmp_msg, "Got signal num %d from ",
				thread_specific_signals[signum]);
	} else {
		sprintf(tmp_msg, "Got unknown signal from ");
	}

	/* now try to restore the pipeline, if the thread was a pipeline */
	if (thread_id > -1) {
		if (strlen(tmp_msg)>1) {
			sprintf(&tmp_msg[strlen(tmp_msg)-1],
					"pipeline thread %d process %d\n",
					thread_id,
					hwapi.pipelines[thread_id].running_process_idx);
		}
		if (pipeline_recover_thread(
				&hwapi.pipelines[thread_id])) {
			aerror("recovering pipeline thread\n");
		}

	} else if (info->si_value.sival_int == -1) {
		strcat(tmp_msg, "the kernel thread\n");
		kernel_timer_recover_thread();
	} else {
		strcat(tmp_msg, "an unkown thread\n");
	}
	return 1;
}

/**
 * This is a thread with priority kernel_prio that synchronously waits for
 * hwapi_pipeline signals (usign sigwaitinfo). All signals except thread-specific
 * ones (SIGSEGV,SIGILL,SIGBUS,SIGFPE) are blocked by all threads except this one.
 * Thread-specific signals are handled by ProcThreads which send a SIGRTMIN+1,
 * SIGRTMIN+2,SIGRTMIN+3,SIGRTMIN+4 (respectively) to this thread, which takes
 * actions accordingly.
 *
 * for signals SIGRTMIN to SIGRTMIN+4, cast the hwapi_pipeline object from this
 * si_value pointer and call hwapi_pipeline_recover_thread(pipeline,
 * pipeline->running_process, TRUE)
 */
static void sigwait_loop(void) {

	int signum;
	sigset_t set;
	siginfo_t info;

        sigfillset(&set);
        sigdelset(&set,TASK_TERMINATION_SIGNAL);
	while(!sigwait_stops) {
		do {
			signum = sigwaitinfo(&set, &info);
		} while (signum == -1 && errno == EINTR);
		if (signum == -1) {
			poserror(errno, "sigwaitinfo");
			goto out;
		}
		hdebug("detected signal %d\n",signum);
		if (signum == KERNEL_SIG_THREAD_SPECIFIC) {
			sigwait_loop_process_thread_signal(&info);
		} else if (signum == SIGINT) {
			printf("Caught SIGINT, exiting\n");
			fflush(stdout);
			goto out;
		} else {
			printf("Got signal %d, exiting\n", signum);
			fflush(stdout);
			goto out;
		}
	}

out:
	go_out();
}


/**
 * Handler for thread-specific signals (SIGSEGV,SIGILL,SIGFPE,SIGBUS).
 * Forwards a signal above SIGRTMIN to myself. Since it is blocked, it will
 * be received by sigwait_loop(), which is runs in the main kernel thread.
 *
 * The thread terminates after exiting the handler.
 */
static void thread_signal_handler(int signum, siginfo_t *info, void *ctx) {
	union sigval value;
	int thread_id;
	int i;

#ifdef PRINT_BT_ON_SIGSEGV
	void *pnt = NULL;
	ucontext_t *context = (ucontext_t*) ctx;
	char **msg;

	pnt = (void*) context->uc_mcontext.gregs[REG_EIP];
	msg = backtrace_symbols(&pnt, 1);
	strcat(sigmsg, msg[0]);
	free(msg);
#endif

	signal_received++;

	hdebug("signal %d received\n",signum);

	/* try to find the thread that caused the signal */

	/** todo: Caution!! is pthread_self() safe in the handler?
	 * it is not async-signal-safe by the standard,
	 * but the signals are synchronous.
	 */
	pthread_t thisthread = pthread_self();

	/* if signum is SIGUSR2, its a task termination signal, just exit */
	if (signum == TASK_TERMINATION_SIGNAL) {
		hdebug("sigusr2 signal. thread=%d\n",thisthread);
		goto cancel_and_exit;
	}

	/* is it a pipeline thread? */
	for (i=0;i<hwapi.machine.nof_cores;i++) {
		if (thisthread == hwapi.pipelines[i].thread) {
			break;
		}
	}
	if (i < hwapi.machine.nof_cores) {
		hdebug("pipeline_idx=%d\n",i);
		thread_id = i;

		/* set the thread to 0 because is terminating */
		hwapi.pipelines[thread_id].thread = 0;
	} else {
		/* it is not, may be it is the kernel timer */
		if (thisthread == single_timer_thread) {
			hdebug("timer thread=%d\n",thisthread);
			thread_id = -1;
		} else {
			/* @TODO: check if it is a status or init thread of any module */

			hdebug("other thread=%d\n",thisthread);
			goto cancel_and_exit;
		}
	}

	/* Now send a signal to the kernel */
	for (i=0;i<N_THREAD_SPECIFIC_SIGNALS;i++) {
		if (thread_specific_signals[i] == signum)
			break;
	}
	hdebug("signal=%d, thread=%d\n",i,thread_id);
	value.sival_int = thread_id<<16 | i;
	if (sigqueue(kernel_pid,
			KERNEL_SIG_THREAD_SPECIFIC,
			value)) {
		poserror(errno, "sigqueue");
	}

cancel_and_exit:
	pthread_exit(NULL);
}

static void check_threads() {
	int i;
	hdebug("nof_cores=%d, timer=%d\n",hwapi.machine.nof_cores,single_timer_thread);
	if (single_timer_thread) {
		if (!pthread_kill(single_timer_thread,0)) {
			aerror("kernel timer still running, killing\n");
			pthread_kill(single_timer_thread, TASK_TERMINATION_SIGNAL);
		}
	}
	for (i=0;i<hwapi.machine.nof_cores;i++) {
		hdebug("thread_%d=%d\n",i,hwapi.pipelines[i].thread);
		if (hwapi.pipelines[i].thread) {
			if (!pthread_kill(hwapi.pipelines[i].thread,0)) {
				aerror_msg("pipeline thread %d still running, killing\n",i);
				pthread_kill(hwapi.pipelines[i].thread, TASK_TERMINATION_SIGNAL);
			}
		}
	}
	hdebug("i=%d\n",i);
}


void open_debug_trace() {
	trace_buffer = open_memstream(&debug_trace_addr, &debug_trace_sz);
	if (!trace_buffer) {
		perror("opening debug_trace\n");
	}
	debug_trace_file = fopen("./out.trace","w");
	if (debug_trace_file) {
		perror("fopen");
	}
}

void write_debug_trace() {
	if (trace_buffer) {
		fclose(trace_buffer);
		trace_buffer = NULL;
	}
	if (debug_trace_addr == NULL) {
		return;
	}
	if (fwrite(debug_trace_addr,1,debug_trace_sz,debug_trace_file) == -1) {
		perror("fwrite");
	}
	debug_trace_addr = NULL;
}


static void go_out() {
	write_debug_trace();
	hdebug("tslot=%d\n",hwapi_time_slot());
	sigwait_stops = 1;
	kernel_timer.stop = 1;
	for (int i=0;i<hwapi.machine.nof_cores;i++) {
		if (hwapi.machine.clock_source == MULTI_TIMER) {
			hwapi.pipelines[i].mytimer.stop = 1;
		} else {
			hwapi.pipelines[i].stop = 1;
		}
	}
	usleep(100000);
	check_threads();
}


int parse_cores_comma_sep(char *str) {
	int i;
	size_t sz;
	char *tok;

	/* assume 10 cores */
	sz = 10;
	assert((core_mapping = malloc(sizeof(int)*sz)));
	i=0;
	tok = strtok(str,",");
	while (tok) {
		core_mapping[i] = atoi(tok);
		tok = strtok(0,",");
		i++;
		if ((size_t) i > sz) {
			sz += 10;
			assert((core_mapping = realloc(core_mapping, sizeof(int)*sz)));
		}
	}
	assert((core_mapping = realloc(core_mapping, sizeof(int)*(size_t)i)));

	return i;
}

int parse_cores_single_array(char *core_init, char *core_end) {
	int c_ini, c_end;
	if (core_init) {
		c_ini = atoi(core_init);
	} else {
		c_ini = 0;
	}
	if (core_end) {
		c_end = atoi(core_end);
	} else {
		return -1;
	}
	core_mapping = malloc(sizeof(int)*((size_t) c_end-(size_t)c_ini));
	for (int i=0;i<(c_end-c_ini);i++) {
		core_mapping[i] = i+c_ini;
	}
	return (c_end-c_ini);
}

/** Parses a string indicating which cores can be used to load modules
 * Valid string formats are:
 * - "N" Just a number, without "," nor ":" means to use core id 0 to N-1
 * - "n1:n2" Indicates that core ids n1 to n2 will be used
 * - "n1,n2,n3" Indicates that core ids n1, n2 and n3 only will be used
 */
int parse_cores(char *str) {
	char *dp;
	char *c;

	dp = index(str,':');
	c = index(str,',');

	if (!c && !dp) {
		return parse_cores_single_array(NULL,str);
	} else if (!c && dp) {
		*dp = '\0';
		dp++;
		return parse_cores_single_array(str,dp);
	} else if (c && !dp) {
		return parse_cores_comma_sep(str);
	} else {
		return -1;
	}
}


int main(int argc, char **argv) {


	if (argc!=4 && argc!=5) {
		printf("Usage: %s ts_us nof_cores path_to_waveform_model [-s]\n",argv[0]);
		return -1;
	}

#ifdef DEBUG_TRACE
	open_debug_trace();
	atexit(write_debug_trace);
#endif

	mlockall(MCL_CURRENT | MCL_FUTURE);

	kernel_pid = getpid();

	timeslot_us = atol(argv[1]);
	if (timeslot_us <= 0) {
		printf("Error invalid timeslot %d\n",(int) timeslot_us);
		exit(0);
	}
	hwapi.machine.nof_cores = parse_cores(argv[2]);
	nof_cores = hwapi.machine.nof_cores;
	if (hwapi.machine.nof_cores <= 0) {
		printf("Error invalid cores %s\n",argv[2]);
		exit(0);
	}

	clock_source = MULTI_TIMER;
	if (argc == 5) {
		if (!strcmp("-s",argv[4])) {
			clock_source = SINGLE_TIMER;
		}
	}

	print_license();
	if (getuid()) {
		printf("Run as root to run in real-time mode\n\n");
	}
	printf("Time slot:\t%d us\nPlatform:\t%d cores\nTimer:\t\t%s\n\n", (int) timeslot_us,
			hwapi.machine.nof_cores,(clock_source==MULTI_TIMER)?"Multi":"Single");

	/* initialize kernel */
	if (kernel_initialize()) {
		aerror("Initiating kernel\n");
		goto clean_and_exit;
	}

	if (hwapi_task_new(NULL,_run_main,argv[3])) {
		hwapi_perror("hwapi_task_new");
		goto clean_and_exit;
	}


	/* the main thread runs the sigwait loop */
	sigwait_loop();



clean_and_exit:
	exit(0);
}


static void print_license() {
	printf("%s  Copyright (C) %d http://flexnets.upc.edu/\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions. See license.txt\n\n",ALOE_VERSION, ALOE_YEAR);
}
