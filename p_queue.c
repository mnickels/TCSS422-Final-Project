#include "p_queue.h"

P_QUEUE_p p_q_constructor() {
	P_QUEUE_p this;
	this = calloc(1, sizeof(P_QUEUE_s));
	if (this) {
		for (int i = 0; i < NUM_PRIORITIES; i++) {
			this->queues[i] = q_constructor(i * 20 + 50);
		}
	}
	return this;
}

void p_q_destructor(P_QUEUE_p * this_ptr) {
	for (int i = 0; i < sizeof((*this_ptr)->queues); i++) {
		q_deconstructor((*this_ptr)->queues[i]);
	}
	free(* this_ptr);
	* this_ptr = NULL;
}

int p_q_is_empty(P_QUEUE_p this) {
	for (int i = 0; i < sizeof(this->queues); i++) {
		if (!q_is_empty(this->queues[i])) {
			return 0;
		}
	}
	return 1;
}

void p_q_enqueue(P_QUEUE_p this, PCB_p pcb) {
	if (this && pcb) {
		q_enqueue(this->queues[pcb->priority], pcb);
	}
}

PCB_p p_q_dequeue(P_QUEUE_p this) {
	if (this) {
		for (int i = 0; i < sizeof(this->queues); i++) {
			if (!q_is_empty(this->queues[i])) {
				return q_dequeue(this->queues[i]);
			}
		}
	}
	return NULL;
}

PCB_p p_q_peek(P_QUEUE_p this) {
	if (this) {
		for (int i = 0; i < sizeof(this->queues); i++) {
			if (!q_is_empty(this->queues[i])) {
				return q_peek(this->queues[i]);
			}
		}
	}
	return NULL;
}
