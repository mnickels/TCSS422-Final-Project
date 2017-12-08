/*
 * TCSS 422 Final Project
 *
 * Mike Nickels
 * David Foster
 * Yaroslav Salo
 */

#ifndef TIMER_H
#define TIMER_H

#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define QUANTUM_SCALAR 50	// .0005 ms

typedef struct timer {
	pthread_t timer_thread;
    int counter;
    pthread_mutex_t mutex;
} timer_s;
typedef timer_s * TIMER_p;

// only for the timer_thread to use
void * timer_run(void *);
void timer_tick(TIMER_p);

// methods for main thread to call
TIMER_p timer_constructor(void);
void timer_deconstructor(TIMER_p *);
void set_timer(TIMER_p , unsigned int);

#endif
