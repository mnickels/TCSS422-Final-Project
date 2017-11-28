// #include "queue.h"
// #include "pcb.h"

// #define NUM_PCB_MAX 200

// void print_queues(QUEUE_p a, QUEUE_p b, QUEUE_p c, QUEUE_p d, QUEUE_p e);

// int main() {
// 	srand(time(0));

// 	QUEUE_p a = q_constructor(10);
// 	QUEUE_p b = q_constructor(10);
// 	QUEUE_p c = q_constructor(10);
// 	QUEUE_p d = q_constructor(10);
// 	QUEUE_p e = q_constructor(10);

// 	for (int i = 0; i < NUM_PCB_MAX	; i++) {
// 		switch (rand() % 5) {
// 			case 0:
// 				q_enqueue(a, pcb_constructor());
// 				break;
// 			case 1:
// 				q_enqueue(b, pcb_constructor());
// 				break;
// 			case 2:
// 				q_enqueue(c, pcb_constructor());
// 				break;
// 			case 3:
// 				q_enqueue(d, pcb_constructor());
// 				break;
// 			case 4:
// 				q_enqueue(e, pcb_constructor());
// 				break;
// 		}
// 	}

// 	for (int i = 0; i < NUM_PCB_MAX	; i++) {
// 		switch (rand() % 5) {
// 			case 0:
// 				q_dequeue(a);
// 				break;
// 			case 1:
// 				q_dequeue(b);
// 				break;
// 			case 2:
// 				q_dequeue(c);
// 				break;
// 			case 3:
// 				q_dequeue(d);
// 				break;
// 			case 4:
// 				q_dequeue(e);
// 				break;
// 		}
// 	}

// 	print_queues(a, b, c, d, e);

// 	q_destructor(&a);
// 	q_destructor(&b);
// 	q_destructor(&c);
// 	q_destructor(&d);
// 	q_destructor(&e);
// }

// void print_queues(QUEUE_p a, QUEUE_p b, QUEUE_p c, QUEUE_p d, QUEUE_p e) {
// 	char * str_a = q_to_string(a);
// 	char * str_b = q_to_string(b);
// 	char * str_c = q_to_string(c);
// 	char * str_d = q_to_string(d);
// 	char * str_e = q_to_string(e);
// 	printf("%s\n%s\n%s\n%s\n%s\n\n", str_a, str_b, str_c, str_d, str_e);
// 	free(str_a);
// 	free(str_b);
// 	free(str_c);
// 	free(str_d);
// 	free(str_e);
// }
