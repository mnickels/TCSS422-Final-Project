#include <assert.h>
#include "timer.h"
#include "os.h"

TIMER_p timer_constructor() {
	TIMER_p timer = calloc(1, sizeof(timer_s));
	if (timer) {
		timer->counter = -1;
		int rc = pthread_mutex_init(&(timer->mutex), NULL);
		assert(rc == 0);
		pthread_create(&(timer->timer_thread), NULL, timer_run, timer);
	}
	return timer;
}

void timer_deconstructor(TIMER_p * timer) {
	if (timer) {
		int rc = pthread_cancel((* timer)->timer_thread);
		assert(rc == 0);

		free(* timer);
		timer = NULL;
	}
}

void * timer_run(void * timer) {
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = QUANTUM_SCALAR;
	for(;;) {
		timer_tick((TIMER_p) timer);
		nanosleep(&ts, NULL);
		printf("Timer tick - %d\n", ((TIMER_p) timer)->counter);
	}
}

void timer_tick(TIMER_p timer) {
	pthread_mutex_lock(&(timer->mutex));
	if (timer->counter == 0) {
		timer->counter--;
		pseudo_ISR(0);
	} else if (timer->counter == -1) {
		// timer is disabled; do nothing
	} else {
		timer->counter--;
	}
	pthread_mutex_unlock(&(timer->mutex));
}

void set_timer(TIMER_p timer, unsigned int quantum) {
	pthread_mutex_lock(&(timer->mutex));
	timer->counter = quantum;
	pthread_mutex_unlock(&(timer->mutex));
}