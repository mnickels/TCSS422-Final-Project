#ifndef IO_DEVICE
#define IO_DEVICE

#define SUCCESS 0
#define PTR_ERR -7

#include "queue.h"
#include <pthread.h>
#include <assert.h>

/**
* IO Device
* 
* io_id: the id of the devide.
* timer: number of cpu cycles until an io interrupt is issued.
* wait_queue: holds the processes that are waiting for io service. 
*/

typedef enum {DEVICE_1 = 1, DEVICE_2 = 2} device_number_t;
typedef struct io_device {
	device_number_t io_id;
	unsigned int timer;
	QUEUE_p wait_queue;
	pthread_mutex_t mutex;
} DEVICE_s ;
typedef DEVICE_s * DEVICE_p;

DEVICE_p device_constructor(device_number_t);
int device_deconstructor(DEVICE_p*);
int device_set_timer(DEVICE_p, unsigned int);
#endif 