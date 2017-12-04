#include "iodevice.h"
#include "pcb.h"

// int main() {
// 	DEVICE_p device1 = device_constructor(1);
// 	//space for testing the queue

// 	PCB_p p1 = pcb_constructor();
// 	PCB_p p2 = pcb_constructor();
// 	PCB_p p3 = pcb_constructor();

// 	PCB_p p4 = pcb_constructor();
// 	PCB_p p5 = pcb_constructor();
// 	PCB_p p6 = pcb_constructor();

// 	DEVICE_p device2 = device_constructor(2);

// 	device_enqueue(device1, p1);
// 	device_enqueue(device1, p2);
// 	device_enqueue(device1, p3);

// 	device_enqueue(device2, p4);
// 	device_enqueue(device2, p5);
// 	device_enqueue(device2, p6);

// 	sleep(5);
// 	device_deconstructor(&device1);
// 	device_deconstructor(&device2);

// 	printf("AFTER DECONSTRUCTION DEVICE 1: %p\n", device1);
// 	printf("AFTER DECONSTRUCTION DEVICE 2: %p\n", device2);
// }
