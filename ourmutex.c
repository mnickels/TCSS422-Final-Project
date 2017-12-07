#include "ourmutex.h"
int mutex_lock(MUTEX_p mutex, PCB_p requesting_process){
    if(!mutex->flag) {
        mutex->current_holder = requesting_process;
        //printf("%s requested lock on mutex M%p - succeeded\n", pcb_simple_to_string(requesting_process), mutex);
        requesting_process->waiting_on_lock = 0;
        mutex->flag = 1;
        return 1;
    } else {
        //put into waiting queue
        //printf("%s requested lock on mutex M%p - blocked by %s\n", pcb_simple_to_string(requesting_process), mutex, pcb_simple_to_string(mutex->current_holder));
        q_enqueue(mutex->waitq, requesting_process);
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
            //printf("%s requested unlock on mutex M%p - succeeded\n", pcb_simple_to_string(mutex->current_holder), mutex);
            mutex->current_holder = NULL;
            if (!q_is_empty(mutex->waitq)) {
                PCB_p temp;
                temp = q_dequeue(mutex->waitq);
                //printf("Waiting process %s, commence lock attempt on mutex M%p\n", pcb_simple_to_string(temp), mutex);
                mutex_lock(mutex, temp);
            }
            return 1;
        } else {
            //printf("%s requested unlock on mutex M%p - not the current holder, which is %s\n", pcb_simple_to_string(requesting_process), mutex, pcb_simple_to_string(mutex->current_holder));
        }
    }
    return 0;
}

MUTEX_p mutex_constructor() {
    MUTEX_p lock = calloc(1, sizeof(MUTEX_s) );
    lock->flag = 0;
    lock->current_holder = NULL;
    lock->waitq = q_constructor(0);
    return lock;
}

int mutex_destructor(MUTEX_p *mutex ) {
    q_destructor( &( ( *mutex)->waitq ) );
    free( *mutex);
    *mutex = NULL;
    return 0;
}