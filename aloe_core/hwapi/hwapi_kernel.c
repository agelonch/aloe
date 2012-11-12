#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "defs.h"
#include "hwapi_time.h"
#include "hwapi_kernel.h"
#include "hwapi_process.h"
#include "hwapi_context.h"
#include "hwapi_task.h"
#include "hwapi.h"
#include "hwapi_timer.h"
#include "nod_module.h"
#include "futex.h"
#include "dummy_test.h"

#define KERNEL_SIG_TIMER SIGRTMIN
#define KERNEL_SIG_THREAD_SPECIFIC SIGUSR1
#define N_THREAD_SPECIFIC_SIGNALS 4
const int thread_specific_signals[N_THREAD_SPECIFIC_SIGNALS] =
	{SIGSEGV, SIGBUS, SIGILL, SIGFPE};


static hwapi_context_t hwapi;
static hwapi_timer_t kernel_timer;
static long int timeslot_us;

static int sigwait_stops = 0;
static int multi_timer_futex;
static pid_t kernel_pid;
static pthread_t single_timer_thread;
static char UNUSED(sigmsg[1024]);


static void go_out();
static void thread_signal_handler(int signum, siginfo_t *info, void *ctx);

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
static int __attribute__((__unused__)) rt_fault(hwapi_process_t *process) {
	aerror("Not yet implemented");
	return -1;
}


inline static void ts_begins_synchronize_rt_control() {
	if (hwapi.machine.rt_fault_opts == RT_FAULT_OPTS_HARD) {
		for (int i=0;i<hwapi.machine.nof_processors;i++) {
			if (!hwapi.pipelines[i].finished) {
				//rt_fault(i);
			}
		}
	}  else {
		for (int i=0;i<hwapi.machine.nof_processors;i++) {
			if (!hwapi.pipelines[i].running_process_idx) {
				//rt_fault(i);
			}
		}
	}
}

static inline void ts_begins_process_periodic_callbacks() {
	/* Call periodic functions */
	for (int i=0;i<hwapi.nof_periodic;i++) {
		if (hwapi.periodic[i].counter==hwapi.periodic[i].period)  {
			hwapi.periodic[i].callback();
			hwapi.periodic[i].counter=0;
		}
		hwapi.periodic[i].counter++;
	}
}

static inline void ts_begin_process_sleep_main_thread() {
	if (hwapi.wake_tslot) {
		if (hwapi_time_slot()>=hwapi.wake_tslot) {
			sem_post(&hwapi.sleep_semaphore);
		}
	}
}

inline void hwapi_kernel_tasks() {

	hwapi_time_ts_inc();

#ifdef rtcontrol
	ts_begins_synchronize_rt_control();
#endif

	ts_begins_process_periodic_callbacks();

	ts_begin_process_sleep_main_thread();
}

static int first_cycle = 0;
/**
 * This function is called by the internal timer, a DAC event or by the sync_slave,
 * after the reception of a synchronization packet.
 */
static void kernel_cycle(void *x, struct timespec *time) {

	if (!first_cycle) {
		hwapi_time_reset_realtime(time);
		first_cycle = 1;
	}
	pipeline_sync_threads();
	hwapi_kernel_tasks();
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
		if (hwapi_task_new_thread(&single_timer_thread, timer_run_thread,
				&kernel_timer, JOINABLE,
				hwapi.machine.kernel_prio, 0)) {
			hwapi_perror("hwapi_task_new_thread");
			return -1;
		}
		break;
	case MULTI_TIMER:
		usleep(100000);
		printf("Starting clocks in %d sec...\n", TIMER_FUTEX_GUARD_SEC);
		clock_gettime(CLOCK_MONOTONIC, &start_time);
		for (int i=0;i<hwapi.machine.nof_processors;i++) {
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

/**
 * Creates the pipelines.
 * @return
 */
inline static int kernel_initialize_create_pipelines() {
	void *(*tmp_thread_fnc)(void*);
	void *tmp_thread_arg;

	pipeline_initialize(hwapi.machine.nof_processors);

	for (int i=0;i<hwapi.machine.nof_processors;i++) {
		hwapi.pipelines[i].id = i;

		if (hwapi.machine.clock_source == MULTI_TIMER) {
			hwapi.pipelines[i].mytimer.period_function =
					pipeline_run_from_timer;
			hwapi.pipelines[i].mytimer.period =
					hwapi.machine.ts_len_us*1000;
			hwapi.pipelines[i].mytimer.arg = &hwapi.pipelines[i];
			hwapi.pipelines[i].mytimer.wait_futex = &multi_timer_futex;
			hwapi.pipelines[i].mytimer.mode = NANOSLEEP;

			hwapi.pipelines[i].mytimer.thread =
					&hwapi.pipelines[i].thread;
			tmp_thread_fnc = timer_run_thread;
			tmp_thread_arg = &hwapi.pipelines[i].mytimer;
		} else {
			tmp_thread_fnc = pipeline_run_thread;
			tmp_thread_arg = &hwapi.pipelines[i];
		}

		if (hwapi_task_new_thread(&hwapi.pipelines[i].thread,
				tmp_thread_fnc, tmp_thread_arg, DETACHABLE,
				hwapi.machine.kernel_prio, CORE_ID(i))) {
			hwapi_perror();
			return -1;
		}
	}

	return 0;
}

/**
 * Creates kernel resources
 */
inline static int kernel_initialize_create_resources() {

	/* initialize shared memory for time sharing with the manager.
	 * hwapi.time structure should then be a pointer to a shared
	 * memory area.
	 * */

	if (sem_init(&hwapi.sleep_semaphore, 0, 0)) {
		poserror(errno, "sem_init");
		return -1;
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
	hwapi.machine.clock_source = MULTI_TIMER;
	hwapi.machine.ts_len_us = timeslot_us;
	hwapi.machine.kernel_prio = 50;
	hwapi_initialize_node(&hwapi, NULL, NULL);

	/* initialize anode */
	//anode_initialize();

	/* create kernel resources */
	if (kernel_initialize_create_resources()) {
		return -1;
	}

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

static void wait_threads() {
	if (single_timer_thread != 0) {
		pthread_join(single_timer_thread, NULL);
		single_timer_thread = 0;
	}
}

/**
 * called after kernel timer caught a synchronous signal.
 * todo: try to recover thread?
 */
static void kernel_timer_recover_thread() {

}

static int sigwait_loop_process_thread_signal(siginfo_t *info) {
	int thread_id, signum;
	strdef(tmp_msg);

	signum = info->si_value.sival_int & 0xffff;
	thread_id = info->si_value.sival_int >> 16;

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
					" pipeline thread %d\n",
					info->si_value.sival_int);
		}
		pipeline_recover_thread(
				&hwapi.pipelines[info->si_value.sival_int]);

	} else if (info->si_value.sival_int == -1) {
		strcat(tmp_msg, " the kernel thread\n");
		kernel_timer_recover_thread();
	} else {
		strcat(tmp_msg, " an unkown thread\n");
	}

	fputs(tmp_msg, stderr);

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
	while(!sigwait_stops) {
		do {
			signum = sigwaitinfo(&set, &info);
		} while (signum == -1 && errno == EINTR);
		if (signum == -1) {
			poserror(errno, "sigwaitinfo");
			goto out;
		}
		switch(signum) {
		case KERNEL_SIG_THREAD_SPECIFIC:
			sigwait_loop_process_thread_signal(&info);
			break;
		case SIGINT:
			printf("Caught SIGINT, exiting\n");
			fflush(stdout);
			goto out;
			break;
		default:
			printf("Got signal %d, exiting\n", signum);
			fflush(stdout);
			goto out;
			break;
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

	/* try to find the thread that caused the signal */

	/** todo: Caution!! is pthread_self() safe in the handler?
	 * it is not async-signal-safe by the standard,
	 * but the signals are synchronous.
	 */
	pthread_t thisthread = pthread_self();

	/* is it a pipeline thread? */
	for (i=0;i<hwapi.machine.nof_processors;i++) {
		if (thisthread == hwapi.pipelines[i].thread) {
			break;
		}
	}
	if (i < hwapi.machine.nof_processors) {
		thread_id = i;
	} else {
		/* it is not, may be it is the kernel timer */
		if (thisthread == single_timer_thread) {
			thread_id = -1;
		} else {
			thread_id = -2;
		}
	}

	/* Now send a signal to the kernel */
	for (i=0;i<N_THREAD_SPECIFIC_SIGNALS;i++) {
		if (thread_specific_signals[i] == signum)
			break;
	}
	value.sival_int = thread_id<<16 | i;
	printf("received sigsegv\n");
	if (sigqueue(kernel_pid,
			KERNEL_SIG_THREAD_SPECIFIC,
			value)) {
		poserror(errno, "sigqueue");
	}
	pthread_cancel(thisthread);
}

static void go_out() {
	sigwait_stops = 1;
	kernel_timer.stop = 1;
	for (int i=0;i<hwapi.machine.nof_processors;i++) {
		if (hwapi.machine.clock_source != MULTI_TIMER) {
			hwapi.pipelines[i].mytimer.stop = 1;
		} else {
			hwapi.pipelines[i].stop = 1;
		}
	}
}

/**
 * if resources opened
 * 1) clear shared memory for init_time
 * 2) clear procThread and sleep semaphores.
 */
static void cleanup() {
	wait_threads();
}


int main(int argc, char **argv) {

	if (argc!=5 && argc!=6) {
		printf("Usage: %s path_to_dummy_lib ts_us nof_modules nof_cores [-s]\n",argv[0]);
		return -1;
	}

	mlockall(MCL_CURRENT | MCL_FUTURE);

	kernel_pid = getpid();

	timeslot_us = atol(argv[2]);
	hwapi.machine.nof_processors = atoi(argv[4]);

	printf("\n----- %s -----\n\n", ALOE_VERSION);
	if (getuid()) {
		printf("Run as root to run in real-time mode\n\n");
	}
	printf("Time slot:\t%d us\nPlatform:\t%d cores\n\n", (int) timeslot_us,
			hwapi.machine.nof_processors);

	/* initialize kernel */
	if (kernel_initialize()) {
		aerror("Initiating kernel\n");
		goto clean_and_exit;
	}

	dummy_test(argv[1],atoi(argv[3]),atoi(argv[4]));

	/* the main thread runs the sigwait loop */
	sigwait_loop();



clean_and_exit:
	cleanup();
	exit(0);
}
