#include "os.h"

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
int trap_flag;

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

    for(;;) {
        if((CPU_PC % 50) == 0){
            printf("PC = %d\n", CPU_PC);
            char* pcbs = pcb_to_string(currentprocess);
            printf("Current process:\n%s\n", pcbs);
            char* pqs = p_q_to_string(readyqueue);
            printf("%s\n...\n\n", pqs);
        }
        
        pthread_mutex_lock(&interrupt_mutex);
        if (interrupt_flag != -1) scheduler();
        pthread_mutex_unlock(&interrupt_mutex);

        runProcess();
        if (trap_flag != -1) scheduler();
        
        if(!(CPU_PC % S)) {
            resetQueue();
        }

        if (!currentprocess->waiting_on_lock) {
            if (CPU_PC >= currentprocess->max_pc) {
                currentprocess->term_count++;
                checkTermination();
                CPU_PC = 0;
            } else {
                CPU_PC++;
            }
        }
        

        if (CPU_PC == 1000) break;
    }
}

// gets called by the timer and IO devices
void psuedo_ISR(interrupt_t interrupt) {
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
    }
    pthread_mutex_unlock(&interrupt_mutex);
}

void scheduler() {
    switch(interrupt_flag){
        case -1:
            switch(trap_flag){
                case -1:
                    //check created queue and add them to readyqueue
                    for (int i = 0; i < createdqueue->length; i++) {
                        PCB_p process = q_dequeue(createdqueue);
                        process->state = ready;
                        p_q_enqueue(readyqueue, process);
                    }

                    dispatcher(0);
                    break;
                case 1:
                    //enqueue process to device 1
                    dispatcher(1);
                    break;
                case 2:
                    //enqueue process to io device 2
                    dispatcher(2);
                    break;
                default:
                    break;
            }
            break;
        case TIMER_INTERRUPT:
            if((rand() % 100) == 50) addPCB(); // context switch has a random chance to add a random process type
            dispatcher(0);
            break;
        case IO1_INTERRUPT:
            PCB_p process = device_dequeue(IOdevice1);
            process->state = ready;
            p_q_enqueue(readyqueue, process);
            break;
        case IO2_INTERRUPT:
            PCB_p process = device_dequeue(IOdevice2);
            process->state = ready;
            p_q_enqueue(readyqueue, process);
            break;
        default:
            break;
    }
    trap_flag = -1;
    interrupt_flag = -1;
    set_timer(timer, get_priority_level_quantum_size(readyqueue, currentprocess->priority));
}

void dispatcher(unsigned int trap_f) {
    if (currentprocess) {
        currentprocess->context->pc = SYS_STACK;
        if (trap_f == 1) {
            currentprocess->state = waiting;
            device_enqueue(IOdevice1, currentprocess);
        } else if (trap_f == 2) {
            currentprocess->state = waiting;
            device_enqueue(IOdevice2, currentprocess);
        } else {
            currentprocess->state = ready;
            p_q_enqueue(readyqueue, currentprocess);
        }
    }
    currentprocess = p_q_dequeue(readyqueue);
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
    scheduler();
}

void runProcess() {
    //check process type and take appropriate action
    int action = 0;
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
                    mutex_lock(currentprocess->mutexR1, currentprocess);
                    if (currentprocess->pair_type == producer) {
                        currentprocess->shared_resource++;
                        printf("Producer %u incremented variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                    } else{
                        printf("Consumer %u read variable %u: %u\n", currentprocess->pid, currentprocess->pair_id, *currentprocess->shared_resource);
                    }
                    break;
                case 2:
                    mutex_unlock(currentprocess->mutexR1, currentprocess);
                    break;
                default:
                    break;
            }
            // if (!currentprocess->waiting_on_lock){
            //     if(currentprocess->pair_type == producer) {
            //         mutex_lock(currentprocess->mutex);
            //         currentprocess->shared_resource++;
            //         mutex_unlock(currentprocess->mutex);
            //     } else {
            //         mutex_lock(currentprocess->mutex);
            //         printf("Consumer Process Pair ID %d: val = %d", currentprocess->pair_id, currentprocess->shared_resource);
            //         mutex_unlock(currentprocess->mutex);
            //     }
            // }
            
            break;
        case(MR):
            action = checkMRAction(currentprocess);
            switch(action) {
                case 0:
                    break;
                case 1:
                    mutex_lock(currentprocess->mutexR1, currentprocess);
                    break;
                case 2:
                    mutex_unlock(currentprocess->mutexR1, currentprocess);
                    break;
                case 3:
                    mutex_lock(currentprocess->mutexR2, currentprocess);
                    printf("Both resources for MR pair %u are used\n", currentprocess->pair_id);
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
        if (CPU_PC == pcb->sync_array_lockR1[i]) {
            return 1;
        } else if (CPU_PC == pcb->sync_array_unlockR1[i]) {
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
    for(int i = 0; i < 4; i++){
        if(CPU_PC == currentprocess->io_1_traps[i]){
            printf("IOInterrupt request for device 1 at PC = %d.\n\n", CPU_PC);
            return 1;
        } else if (CPU_PC == currentprocess->io_2_traps[i]){
            printf("IOInterrupt request for device 2 at PC = %d.\n\n", CPU_PC);
            return 2;
        }
    }
    return -1;
}

void checkTermination() {
    // check if the current process can be terminated, and its term_count == 0
    if (currentprocess->terminate && currentprocess->term_count >= currentprocess->terminate) {
        currentprocess->state = halted;
        q_enqueue(terminatedqueue, currentprocess);
        currentprocess = NULL;
        scheduler();
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
            q_enqueue(createdqueue, pcb);
            q_enqueue(createdqueue, pcb2);
            PCB_COUNT += 2;
            break;
        default:
            break;
    }
    scheduler();
}

void resetQueue() {
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
}