#include "queue.h"

QUEUE_p q_constructor(unsigned int quantum_size) {
	QUEUE_p this;
	this = calloc(1, sizeof(QUEUE_s));
	if (this) {
		this->quantum_size = quantum_size;
		this->head = NULL;
		this->tail = NULL;
		this->length = 0;
	}
	return this;
}

void q_destructor(QUEUE_p * this_ptr) {
	while (!q_is_empty(* this_ptr)) {
		PCB_p temp = q_dequeue(*this_ptr);
		pcb_deconstructor(&temp);
	}
	free(* this_ptr);
	* this_ptr = NULL;
}

int q_is_empty(QUEUE_p this) {
	return this->length <= 0;
}

void q_enqueue(QUEUE_p this, PCB_p pcb) {
	q_node * node;
	node = calloc(1, sizeof(q_node));
	if (node) {
		node->next = NULL;
		node->pcb = pcb;
	}
	if (!this->head) {
		this->head = node;
	} else {
		this->tail->next = node;
	}
	this->tail = node;
}

PCB_p q_dequeue(QUEUE_p this) {
	q_node * temp;
	PCB_p to_return = NULL;
	if (this->head) {
		temp = this->head;
		this->head = this->head->next;
		to_return = temp->pcb;
		free(temp);
	}
	return to_return;
}

PCB_p q_peek(QUEUE_p this) {
	return this->head;
}