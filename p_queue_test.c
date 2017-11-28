// #include "pcb.h"
// #include "queue.h"
// #include "p_queue.h"

// #define NUM_PCB_MAX 200

// void is_empty(P_QUEUE_p);

// int main() {
// 	srand(time(0));

// 	P_QUEUE_p pq = p_q_constructor();

// 	is_empty(pq);

// 	for (int i = 0; i < 10; i++) {
// 		PCB_p p = pcb_constructor();
// 		p->priority = rand() % NUM_PRIORITIES;
// 		p_q_enqueue(pq, p);
// 		char * ps = pcb_to_string(p);
// 		printf("%s\n", ps);
// 		free(ps);
// 		char * pqs = p_q_to_string(pq);
// 		printf("%s\n", pqs);
// 		free(pqs);
// 	}

// 	for (int i = 10; i < NUM_PCB_MAX; i++) {
// 		PCB_p p = pcb_constructor();
// 		p->priority = rand() % NUM_PRIORITIES;
// 		p_q_enqueue(pq, p);
// 	}

// 	is_empty(pq);

// 	for (int i = 0; i < NUM_PCB_MAX; i++) {
// 		PCB_p temp = p_q_dequeue(pq);
// 		pcb_deconstructor(&temp);
// 	}

// 	char * pqs = p_q_to_string(pq);
// 	printf("%s\n", pqs);
// 	free(pqs);

// 	is_empty(pq);

// 	p_q_destructor(&pq);
// }

// void is_empty(P_QUEUE_p pq) {
// 	printf("Empty? : %d\n", p_q_is_empty(pq));
// }