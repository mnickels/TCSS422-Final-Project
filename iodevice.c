#include "iodevice.h"

DEVICE_p device_constructor(device_number_t io_id) {
	
	DEVICE_p device;
	QUEUE_p wait_queue;
	device = calloc(1, sizeof(DEVICE_s));
	wait_queue = q_constructor(0); 
	
	if(device && wait_queue) {
		device->io_id = io_id;
		device->timer = 0;
		device->wait_queue = wait_queue;
		int rc = pthread_mutex_init(&(device->mutex), NULL);
		assert(rc == 0);
		printf("RC VALUE: %d\n", rc);
	}
	return device;
}

int device_deconstructor(DEVICE_p  *device_ptr) {
	if(! *device_ptr) return PTR_ERR;

	q_destructor( &( ( *device_ptr )->wait_queue ) );
	free( *device_ptr);
	*device_ptr = NULL;
	return SUCCESS; 
} 

int device_set_timer(DEVICE_p device_ptr, unsigned int time) {
	if(!device_ptr) return PTR_ERR;
	device_ptr->timer = time;
	return SUCCESS;
}

