#include "iodevice.h"
#include "pcb.h"

int main() {
	DEVICE_p device1 = device_constructor(1);
	printf("AFTER CONSTRUCTION\nID: %u\n", device1->io_id);
	printf("TIMER: %u\n", device1->timer);
	//space for testing the queue


	PCB_p p1 = pcb_constructor();
	
	device_set_timer(device1, 100);
	printf("AFTER SET\nTIMER: %u\n", device1->timer);
	

	DEVICE_p device2 = device_constructor(2);
	printf("AFTER CONSTRUCTION\nID: %u\n", device2->io_id);
	printf("TIMER: %u\n", device2->timer);


	device_set_timer(device2, 200);
	printf("AFTER SET\nTIMER: %u\n", device2->timer);
	
	device_deconstructor(&device1);
	device_deconstructor(&device2);

	printf("AFTER DECONSTRUCTION DEVICE 1: %p\n", device1);	
	printf("AFTER DECONSTRUCTION DEVICE 2: %p\n", device2);	
}