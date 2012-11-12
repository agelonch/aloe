#ifndef FUTEX_H_
#define FUTEX_H_

#include <limits.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

#define futex_wait(a) syscall(SYS_futex, a, FUTEX_WAIT, 0, NULL, NULL, NULL)
#define futex_wake(a) syscall(SYS_futex, a, FUTEX_WAKE, INT_MAX, NULL, NULL, NULL)

#endif /* FUTEX_H_ */
