#include "ourmutex.h"

MUTEX_p mutex_constructor(MUTEX_p mutex);
int mutex_init(MUTEX_p mutex);
int mutex_destructor(MUTEX_p mutex);

int mutex_lock(MUTEX_p mutex, PCB_p requesting_process){
    if(!mutex->flag) {
        mutex->current_holder = requesting_process;
        mutex->flag = 1;
        return 1;
    } else{
        //put into waiting queue
        q_enqueue(mutex->waiting_queue, requesting_process);
        requesting_process->waiting_on_lock = 1;
        return 0;
    }
}

int mutex_trylock(MUTEX_p mutex, PCB_p requesting_process) {
    if(!mutex->flag) {
        mutex->current_holder = requesting_process;
        mutex->flag = 1;
        return 1;
    }
    
    return 0;
}


int mutex_unlock(MUTEX_p mutex, PCB_p requesting_process) {
    if (mutex->flag) {
        if (mutex->current_holder == requesting_process){
            mutex->flag = 0;
            if (!q_isempty(mutex->waitq)) {
                PCB_p temp;
                temp = q_dequeue(mutex->waitq);
                temp->waiting_on_lock = 0;
                mutex_lock(mutex, temp);
                return 1;
            }
        }
    }
    
    return 0;
}


MUTEX_p mutex_constructor() {
    MUTEX_p lock = calloc(1, sizeof(MUTEX_s));
    lock->flag = 0;
    lock->current_holder = NULL;
    lock->waitq = q_constructor(0);
    return MUTEX_p;
}

int mutex_destructor( *MUTEX_p mutex ) {
    q_destructor( &( ( *mutex)->waitq ) );
    free( *mutex);
    *mutex = NULL;
    return 0;
}