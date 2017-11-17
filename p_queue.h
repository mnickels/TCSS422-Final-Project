#ifndef PRIO_QUEUE_HEADER
#define PRIO_QUEUE_HEADER

#include "queue.h"
#include "pcb.h"

typedef struct p_q_s {
	QUEUE_p queues[NUM_PRIORITIES];
} P_QUEUE_s;

typedef P_QUEUE_s * P_QUEUE_p;

P_QUEUE_p p_q_constructor(unsigned int);

void p_q_destructor(P_QUEUE_p *);

int p_q_is_empty(P_QUEUE_p);

void p_q_enqueue(P_QUEUE_p, PCB_p);

PCB_p p_q_dequeue(P_QUEUE_p);

PCB_p p_q_peek(P_QUEUE_p);

#endif
