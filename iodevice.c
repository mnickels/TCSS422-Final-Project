#include "iodevice.h"
#include "timer.h"
#include "os.h"

DEVICE_p device_constructor(device_number_t io_id) {
	
	DEVICE_p device;
	QUEUE_p wait_queue;
	device = calloc(1, sizeof(DEVICE_s));
	wait_queue = q_constructor(0); 
	
	if(device && wait_queue) {
		device->io_id = io_id;
		device->ready = 1;
		device->wait_queue = wait_queue;
		int rc = pthread_mutex_init(&(device->mutex), NULL);
		assert(rc == 0);
		pthread_create(&(device->device_thread), NULL, device_run, device);
		return device;
	} 
	return NULL;
//	return device;
}

int device_deconstructor(DEVICE_p  *device_ptr) {
	if(! *device_ptr) return PTR_ERR;

	int rc = pthread_cancel((*device_ptr)->device_thread);
	assert(rc == 0);	
	q_destructor( &( ( *device_ptr )->wait_queue ) );

	free( *device_ptr);
	*device_ptr = NULL;
	return SUCCESS; 
} 

int device_enqueue(DEVICE_p device_ptr, PCB_p pcb_ptr) {
	if(!device_ptr || !pcb_ptr) return PTR_ERR;
	pthread_mutex_lock(&(device_ptr->mutex));
	//printf("Device num: %d ENQ LOCKED pcb_id: %d\n", device_ptr->io_id, pcb_ptr->pid);
	q_enqueue(device_ptr->wait_queue, pcb_ptr); 
	pthread_mutex_unlock(&(device_ptr->mutex));
	//printf("Device num: %d ENQ UNLOCKED pcb_id: %d\n", device_ptr->io_id, pcb_ptr->pid);
	return SUCCESS;
}

PCB_p device_dequeue(DEVICE_p device_ptr) {
	if(!device_ptr) return NULL;
	pthread_mutex_lock(&(device_ptr->mutex));
	//printf("Device num: %d DEQ LOCKED\n", device_ptr->io_id);
	PCB_p temp = q_dequeue(device_ptr->wait_queue);
	device_ptr->ready = 1;
	//printf("In deque pcb_pid: %d\n ", temp->pid);
	pthread_mutex_unlock(&(device_ptr->mutex));
	//printf("Device num: %d DEQ UNLOCKED\n", device_ptr->io_id);
	return temp;
}

void *device_run(void * device_ptr) {
	DEVICE_p d = (DEVICE_p) device_ptr;
	struct timespec ts;
	ts.tv_sec = 3;	// 3 seconds for a single IO interrupt to occur
	ts.tv_nsec = 0;
	for(;;) {
		if (d->ready && d->wait_queue->length) {
			nanosleep(&ts, NULL);
			d->ready = 0; 
			pseudo_ISR(d->io_id);
		}
		// printf("Device num: %d in the runner\n", d->io_id);
		// sleep(1);
		// int temp = device_dequeue(d);
		// pseudo_ISR(IO_INTERRUPT);
		// printf("After waking up the pid of dequed process is: *%d*\n", temp);
	}
}