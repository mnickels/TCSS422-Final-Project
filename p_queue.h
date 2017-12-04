#ifndef PRIO_QUEUE_HEADER
#define PRIO_QUEUE_HEADER

#include "queue.h"
#include "pcb.h"

#define MAX_P_Q_STRING_LEN 4096

typedef struct p_q_s {
	QUEUE_p queues[NUM_PRIORITIES];
} P_QUEUE_s;

typedef P_QUEUE_s * P_QUEUE_p;

P_QUEUE_p p_q_constructor();

void p_q_destructor(P_QUEUE_p *);

int p_q_is_empty(P_QUEUE_p);

void p_q_enqueue(P_QUEUE_p, PCB_p);

PCB_p p_q_dequeue(P_QUEUE_p);

PCB_p p_q_peek(P_QUEUE_p);

unsigned int get_priority_level_quantum_size(P_QUEUE_p, unsigned int);

char * p_q_to_string(P_QUEUE_p);

#endif
