#include "p_queue.h"
#include "pcb.h"

typedef struct mutex {
    unsigned int flag:1;
    PCB_p current_holder;
    QUEUE_p waitq;
} MUTEX_s;
typedef MUTEX_s * MUTEX_p;

MUTEX_p mutex_constructor(MUTEX_p mutex);
int mutex_init(MUTEX_p mutex);
int mutex_destructor(MUTEX_p mutex);
int mutex_lock(MUTEX_p mutex, PCB_p requesting_process);
int mutex_trylock(MUTEX_p mutex, PCB_p requesting_process);
int mutex_unlock(MUTEX_p mutex);