#ifndef FIFO_QUEUE_HEADER
#define FIFO_QUEUE_HEADER

#include "pcb.h"
#include <stdlib.h>

typedef struct q_node_s {
	PCB_p pcb;
	struct q_node_s * next;
} q_node;

typedef struct q_s {
	q_node * head;
	q_node * tail;
	unsigned int length;
	unsigned int quantum_size;
} QUEUE_s;

typedef QUEUE_s * QUEUE_p;

QUEUE_p q_constructor(unsigned int);

void q_destructor(QUEUE_p *);

int q_is_empty(QUEUE_p);

void q_enqueue(QUEUE_p, PCB_p);

PCB_p q_dequeue(QUEUE_p);

PCB_p q_peek(QUEUE_p);

#endif