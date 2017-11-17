#include "queue.h"
#include <pthread.h>

typedef struct mutex {
    PCB* process;
    q_s waitq;
}
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg);
int pthread_join(pthread_t thread, void** value_ptr);
int pthread_mutex_trylock(pthread_mutex_t* mutex);
int pthread_mutex_timedlock(pthread_mutex_t* mutex, struct timespec* abs_timeout);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
int pthread_cond_signal(pthread_cond_t* cond);