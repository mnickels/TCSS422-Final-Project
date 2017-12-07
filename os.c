#include "os.h"


MUT_p mut_res_arr[(MAX_PCB + 1)/2];
unsigned int mut_res_idx = 0;
unsigned int switch_count = 0;
P_QUEUE_p readyqueue;
QUEUE_p createdqueue;
QUEUE_p terminatedqueue;
DEVICE_p IOdevice1;
DEVICE_p IOdevice2;
TIMER_p timer;
PCB_p currentprocess;
unsigned int CPU_PC;
unsigned int SYS_STACK;
int PCB_COUNT;
interrupt_t interrupt_flag;
pthread_mutex_t interrupt_mutex;
trap_t trap_flag;
unsigned int s_counter;

int main() {
    srand(0);
    readyqueue = p_q_constructor();
    createdqueue = q_constructor(0);
    terminatedqueue = q_constructor(0);
    IOdevice1 = device_constructor(DEVICE_1);
    IOdevice2 = device_constructor(DEVICE_2);
    timer = timer_constructor();
    CPU_PC = 0;
    PCB_COUNT = 0;
    interrupt_flag = -1;
    pthread_mutex_init(&interrupt_mutex, NULL);
    trap_flag = -1;
    generateInitialPCBs();
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = QUANTUM_SCALAR;
    s_counter = S;

    for(;;) {
        nanosleep(&ts, NULL);
        // printf("main loop @ PC=0x%04X\n", CPU_PC);

        if (!currentprocess->waiting_on_lock) {
            runProcess();
        }

        if (!currentprocess->waiting_on_lock) {
            if (CPU_PC >= currentprocess->max_pc) {
                currentprocess->term_count++;
                CPU_PC = 0;
                checkTermination();
            } else {
                CPU_PC++;
            }
        }
        
        pthread_mutex_lock(&interrupt_mutex);
        if (interrupt_flag != NO_INTERRUPT) scheduler();
        else if (trap_flag != NO_TRAP) scheduler();
        pthread_mutex_unlock(&interrupt_mutex);
    }
}

// gets called by the timer and IO devices
void pseudo_ISR(interrupt_t interrupt) {
    pthread_mutex_lock(&interrupt_mutex);
    switch (interrupt_flag) {
        case NO_INTERRUPT:
        case IO2_INTERRUPT:
            interrupt_flag = interrupt;
            break;
        case IO1_INTERRUPT:
            if (interrupt == TIMER_INTERRUPT) {
                interrupt_flag = interrupt;
            }
            break;
        default:
            break;
    }
    pthread_mutex_unlock(&interrupt_mutex);
    switch (interrupt) {
        case NO_INTERRUPT:
            //printf("pseudo-ISR called with no interrupt specified!!!\n");
            break;
        case TIMER_INTERRUPT:
            //printf("Timer interrupt occurred\n");
            break;
        default:
            //printf("IO interrupt occurred on IO device %d\n", interrupt);
            break;
    }
}

void scheduler() {
    PCB_p process;
    SYS_STACK = CPU_PC;
    switch(interrupt_flag){
        case TIMER_INTERRUPT:
        case NO_INTERRUPT:      // NO_INTERRUPT means this is the first run of the program
            // chance to add random PCB
            if((rand() % 50) == 0) {
                printf("Added a new pcb\n");
                addPCB(); // context switch has a random chance to add a random process type
            }
            //check created queue and add them to readyqueue
            if (!q_is_empty(createdqueue)) {
                while(!q_is_empty(createdqueue)) {
                    process = q_dequeue(createdqueue);
                    process->state = ready;
                    p_q_enqueue(readyqueue, process);
                }
                char * qs = p_q_to_string(readyqueue);
                //printf("Ready Queue after pcbs added:\n%s\n\n", qs);
                free(qs);
            }
            // context switch
            dispatcher();
            break;
        case IO1_INTERRUPT:
            interrupt_flag = -1;
            process = device_dequeue(IOdevice1);
            if (process) {
                process->state = ready;
                //process->context->pc++;
                p_q_enqueue(readyqueue, process);
            }
            break;
        case IO2_INTERRUPT:
            interrupt_flag = -1;
            process = device_dequeue(IOdevice2);
            if (process) {
                process->state = ready;
                //process->context->pc++;
                p_q_enqueue(readyqueue, process);
            }
            break;
        default:
            break;
    }
    trap_flag = -1;
    interrupt_flag = -1;
    CPU_PC = SYS_STACK;
    set_timer(timer, get_priority_level_quantum_size(readyqueue, currentprocess->priority));
}

void dispatcher() {
    char * temp;
    if (currentprocess) {
        temp = pcb_simple_to_string(currentprocess);
        printf("Current process %s ", temp);
        free(temp);
        currentprocess->context->pc = SYS_STACK;
        if (trap_flag == IO1_TRAP) {
            currentprocess->state = waiting;
            device_enqueue(IOdevice1, currentprocess);
        } else if (trap_flag == IO2_TRAP) {
            currentprocess->state = waiting;
            device_enqueue(IOdevice2, currentprocess);
        } else {
            currentprocess->state = ready;
            currentprocess->priority++;
            p_q_enqueue(readyqueue, currentprocess);
        }
    }

    // check S
    s_counter--;
    if (!s_counter) {
        resetQueue();
        s_counter = S;
    }

    currentprocess = p_q_dequeue(readyqueue);
    temp = pcb_simple_to_string(currentprocess);
    printf("context switch to %s\n", temp);
    free(temp);
    currentprocess->state = running;
    SYS_STACK = currentprocess->context->pc;
}

void generateInitialPCBs() {
    int mod = 0;
    for(int i = 0; i < 8; i++){
        mod = i % 4;
        switch (mod) {
            case(0):
                createPCB(IO);
                break;
            case(1):
                createPCB(CI);
                break;
            case(2):
                createPCB(PrCo);
                break;
            case(3):
                createPCB(MR);
                break;
            default:
                break;
        }
    }
    PCB_p process;
    if (!q_is_empty(createdqueue)) {
        while(!q_is_empty(createdqueue)) {
            process = q_dequeue(createdqueue);
            printf("%s\n", pcb_to_string(process));
            process->state = ready;
            p_q_enqueue(readyqueue, process);
        }
        char * qs = p_q_to_string(readyqueue);
        printf("Ready Queue after pcbs added:\n%s\n\n", qs);
        free(qs);
    }
    scheduler();
}

void runProcess() {
    //check process type and take appropriate action
    int action = 0;
    int lock_result;
    char * temp;
    switch (currentprocess->p_type) {
        case(IO):
            trap_flag = checkIOTrap();
            break;
        case(CI):
            break;
        case(PrCo):
            action = checkPCAction(currentprocess);
            switch(action) {
                case 0:
                    break;
                case 1:
                    if(((MUTEX_p)currentprocess->mutexR1)->current_holder == currentprocess) {
                        //printf("Mutex lock acquired after waiting\n");
                        if(currentprocess->pair_type == producer){
                            (*currentprocess->shared_resource)++;
                            printf("Producer %u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                        } else {
                            printf("Consumer %u read variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                        }
                    } else {
                        lock_result = mutex_lock(currentprocess->mutexR1, currentprocess);
                        if (lock_result) {
                            if(currentprocess->pair_type == producer){
                                (*currentprocess->shared_resource)++;
                                printf("Producer %u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                            } else {
                                printf("Consumer %u read variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                            }
                        }
                    }
                    
                    break;
                case 2:
                    lock_result = mutex_unlock(currentprocess->mutexR1, currentprocess);
                    if (!lock_result) {
                        temp = pcb_simple_to_string(currentprocess);
                        printf("ERROR!!! %s tried to unlock its mutex but was already unlocked\n", temp);
                        free(temp);
                    }
                    break;
                default:
                    break;
            }
            break;
        case(MR):
            action = checkMRAction(currentprocess);
            switch(action) {
                case 0:
                    break;
                case 1:
                    if (((MUTEX_p)currentprocess->mutexR1)->current_holder == currentprocess) {
                        // printf("Mutex R1 lock was acquired after waiting\n");
                        if (FORCE_DEADLOCK && currentprocess->pair_type == B) {
                            (*currentprocess->shared_resource)++;
                            printf("Mutual Resource P%u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                        }
                    } else {
                        lock_result = mutex_lock(currentprocess->mutexR1, currentprocess);
                        if (lock_result && (FORCE_DEADLOCK && currentprocess->pair_type == B)) {
                            (*currentprocess->shared_resource)++;
                            printf("Mutual Resource P%u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                        }
                    }
                    break;
                case 2:
                    mutex_unlock(currentprocess->mutexR1, currentprocess);
                    break;
                case 3:
                    if(((MUTEX_p)currentprocess->mutexR2)->current_holder == currentprocess) {
                        // printf("Mutex R2 lock was acquired after waiting\n");
                        if (!FORCE_DEADLOCK || currentprocess->pair_type == A) {
                            (*currentprocess->shared_resource)++;
                            printf("Mutual Resource P%u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                        }
                    } else {
                        lock_result = mutex_lock(currentprocess->mutexR2, currentprocess);
                        if (lock_result && (!FORCE_DEADLOCK || currentprocess->pair_type == A)) {
                            (*currentprocess->shared_resource)++;
                            printf("Mutual Resource P%u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                        }
                    }
                    break;
                case 4:
                    mutex_unlock(currentprocess->mutexR2, currentprocess);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

int checkPCAction(PCB_p pcb) {
    for(int i = 0; i < 4; i++) {
        //printf("Lock[%d] = %d, Unlock = %d, CPU_PC = %d\n", i, pcb->sync_array_lockR1[i], pcb->sync_array_unlockR1[i], CPU_PC);
        if (CPU_PC == pcb->sync_array_lockR1[i]) {
            //printf("Lock with CPU_PC = %d and sync[i] = %d\n", CPU_PC, pcb->sync_array_lockR1[i]);
            return 1;
        } else if (CPU_PC == pcb->sync_array_unlockR1[i]) {
            //printf("Unlock with CPU_PC = %d and sync[i] = %d\n", CPU_PC, pcb->sync_array_unlockR1[i]);
            return 2;
        }
    }
    return 0;
}

int checkMRAction(PCB_p pcb) {
    for (int i = 0; i < 4; i++) {
        if (CPU_PC == pcb->sync_array_lockR1[i]) {
            return 1;
        } else if (CPU_PC == pcb->sync_array_unlockR1[i]) {
            return 2;
        } else if (CPU_PC == pcb->sync_array_lockR2[i]) {
            return 3;
        } else if (CPU_PC == pcb->sync_array_unlockR2[i]) {
            return 4;
        }
    }
    return 0;
}

int checkIOTrap() {
    char * temp;
    for(int i = 0; i < 4; i++){
        if(CPU_PC == currentprocess->io_1_traps[i]){
            temp = pcb_simple_to_string(currentprocess);
            printf("%s -- Trap request for device 1 at PC = %d.\n\n", temp, CPU_PC);
            free(temp);
            return 1;
        } else if (CPU_PC == currentprocess->io_2_traps[i]){
            temp = pcb_simple_to_string(currentprocess);
            printf("%s -- Trap request for device 2 at PC = %d.\n\n", temp, CPU_PC);
            free(temp);
            return 2;
        }
    }
    return -1;
}

void checkTermination() {
    // check if the current process can be terminated, and its term_count == 0
    if (currentprocess->terminate && currentprocess->term_count >= currentprocess->terminate) {
        char * pcbs = pcb_to_string(currentprocess);
        printf("term count: %d and max terms: %d  and max_pc: %d\n", currentprocess->term_count, currentprocess->terminate, currentprocess->max_pc);
        printf("Process to be terminated: %s\n", pcbs);
        free(pbcs);
        currentprocess->state = halted;
        q_enqueue(terminatedqueue, currentprocess);
        currentprocess = NULL;
        pthread_mutex_lock(&interrupt_mutex);
        interrupt_flag = TIMER_INTERRUPT;
        scheduler();
        pthread_mutex_unlock(&interrupt_mutex);
    }
    // check terminate queue size and empty if it reaches a certain size
    if (terminatedqueue->length >= 5) {
        while (!q_is_empty(terminatedqueue)) {
            PCB_p temp = q_dequeue(terminatedqueue);
            pcb_deconstructor(&temp);
        }
    }
}

void addPCB() {
    int ptype = rand() % 4;
    switch (ptype) {
        case(0):
            createPCB(IO);
            //scheduler();
            break;
        case(1):
            createPCB(CI);
            //scheduler();
            break;
        case(2):
            createPCB(PrCo);
            //scheduler();
            break;
        case(3):
            createPCB(MR);
            //scheduler();
            break;
        default:
            break;
    }
}

void createPCB(enum process_type ptype) {
    PCB_p pcb;
    PCB_p pcb2;
    switch (ptype) {
        case(IO):
            pcb = pcb_constructor(IO);
            q_enqueue(createdqueue, pcb);
            PCB_COUNT++;
            break;
        case(CI):
            pcb = pcb_constructor(CI);
            q_enqueue(createdqueue, pcb);
            PCB_COUNT++;
            break;
        case(PrCo):
            pcb = pcb_constructor(PrCo);
            pcb2 = pcb_constructor(PrCo);
            assignPCPair(pcb, pcb2);
            q_enqueue(createdqueue, pcb);
            q_enqueue(createdqueue, pcb2);
            PCB_COUNT += 2;
            break;
        case(MR):
            pcb = pcb_constructor(MR);
            pcb2 = pcb_constructor(MR);
            assignMRPair(pcb, pcb2);
            MUT_p m_pair = mut_constructor(pcb, pcb2);
            mut_res_arr[mut_res_idx++] = m_pair;
            q_enqueue(createdqueue, pcb);
            q_enqueue(createdqueue, pcb2);
            PCB_COUNT += 2;
            break;
        default:
            break;
    }
    //scheduler();
}

void resetQueue() {
    printf("S=%d iterations have passed - moving all processes to priority 0.\n", S);
    char * pq_string = p_q_to_string(readyqueue);
    printf(pq_string);
    free(pq_string);
    QUEUE_p temp = q_constructor(0);
    // move all pcbs out of the readyqueue
    while (!p_q_is_empty(readyqueue)) {
        q_enqueue(temp, p_q_dequeue(readyqueue));
    }
    // set all pcbs' priorities to zero and re-add to readyqueue
    while (!q_is_empty(temp)) {
        PCB_p temp_pcb = q_dequeue(temp);
        temp_pcb->state = ready;
        temp_pcb->priority = 0;
        p_q_enqueue(readyqueue, temp_pcb);
    }
    q_destructor(&temp);
    pq_string = p_q_to_string(readyqueue);
    printf(pq_string);
    free(pq_string);
}

void deadlock_monitor() {
    for(int i = 0; i  < mut_res_idx; i++) {
        MUT_p temp = mut_res_arr[i];
        if(temp->a->waiting_on_lock)
    }
}