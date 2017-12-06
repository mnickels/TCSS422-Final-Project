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
	for (int i = 0; i < NUM_PRIORITIES; i++) {
		q_destructor(&(*this_ptr)->queues[i]);
	}
	free(* this_ptr);
	* this_ptr = NULL;
}

int p_q_is_empty(P_QUEUE_p this) {
	for (int i = 0; i < NUM_PRIORITIES; i++) {
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
		for (int i = 0; i < NUM_PRIORITIES; i++) {
			if (!q_is_empty(this->queues[i])) {
				return q_dequeue(this->queues[i]);
			}
		}
	}
	return NULL;
}

PCB_p p_q_peek(P_QUEUE_p this) {
	if (this) {
		for (int i = 0; i < NUM_PRIORITIES; i++) {
			if (!q_is_empty(this->queues[i])) {
				return q_peek(this->queues[i]);
			}
		}
	}
	return NULL;
}

unsigned int get_priority_level_quantum_size(P_QUEUE_p this, unsigned int priority) {
	return get_quantum_size(this->queues[priority]);
}

char * p_q_to_string(P_QUEUE_p this) {
	if (!this) return NULL;

    char * str = (char *) calloc(MAX_P_Q_STRING_LEN, sizeof(char));
    char * tracker = str;
    char buffer[MAX_Q_STRING_LEN + 2];
    int total_pcbs = 0;

    tracker += sprintf(tracker, "PQ:\n");
    for (int i = 0; i < NUM_PRIORITIES; i++) {
    	sprintf(buffer, "\t%s\n", q_to_string(this->queues[i]));
    	tracker += sprintf(tracker, buffer, i);
    	total_pcbs += this->queues[i]->length;
    }
    tracker += sprintf(tracker, "PCB count: %d", total_pcbs);

    return str;
}
