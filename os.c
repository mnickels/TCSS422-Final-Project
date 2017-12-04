
#include "os.h"

P_QUEUE_p readyqueue;
QUEUE_p createdqueue;
DEVICE_p IOdevice1;
DEVICE_p IOdevice2;
PCB_p currentprocess;
unsigned int PC;
int PCB_COUNT;

int main() {
    srand(0);
    readyqueue = p_q_constructor();
    createdqueue = q_constructor(0);
    IOdevice1 = device_constructor(DEVICE_1);
    IOdevice2 = device_constructor(DEVICE_2);
    //initialize timer
    PC = 0;
    PCB_COUNT = 0;
    generateInitialPCBs();
    
    for(;;) {
        if((PC % 50) == 0){
            printf("PC = %d\n", PC);
            char* pcbs = pcb_to_string(currentprocess);
            printf("Current process:\n%s\n", pcbs);
            char* pqs = p_q_to_string(readyqueue);
            printf("%s\n...\n\n", pqs);
        }
        runProcess();
        checkIOInterrupt();
        checkTermination();
        if(PC == S){
            resetQueue();
        }
        if (PC == 1000) break;
        //if(timer_flag){
            //context switch
        //}
    }
}

void scheduler() {

}

void dispatcher() {

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
    currentprocess = p_q_dequeue(readyqueue);
    currentprocess->state = ready;
}

void runProcess() {
    PC++;
    //check process type and take appropriate action
    switch (currentprocess->p_type) {
        case(IO):
            //check for interrupt?
            break;
        case(CI):
            break;
        case(PrCo):
            if(currentprocess->pair_type == producer) {
                pthread_mutex_lock(currentprocess->mutex);
                currentprocess->shared_resource++;
                pthread_mutex_unlock(currentprocess->mutex);
            } else {
                pthread_mutex_lock(currentprocess->mutex);
                printf("Consumer Process Pair ID %d: val = %d", currentprocess->pair_id, currentprocess->shared_resource);
                pthread_mutex_unlock(currentprocess->mutex);
            }
            break;
        case(MR):
            //do mutual resource stuff
            break;
        default:
            break;
    }
}

int checkIOInterrupt() {
    for(int i = 0; i < 4; i++){
        if(PC == currentprocess->io_1_traps[i]){
            printf("IOInterrupt request for device 1 at PC = %d.\n\n", PC);
            return 1;
        } else if (PC == currentprocess->io_2_traps[i]){
            printf("IOInterrupt request for device 2 at PC = %d.\n\n", PC);
            return 2;
        }
    }
    return -1;
}

int checkTermination() {
    if (currentprocess->terminate == 0){
        //do the random thing and add to terminate queue if it triggers
    }
    //check terminate queue size and empty if it reaches a certain size ?5?
    return 0;
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
            assignPair(pcb, pcb2);
            q_enqueue(createdqueue, pcb);
            q_enqueue(createdqueue, pcb2);
            PCB_COUNT += 2;
            break;
        case(MR):
            pcb = pcb_constructor(MR);
            pcb2 = pcb_constructor(MR);
            assignPair(pcb, pcb2);
            q_enqueue(createdqueue, pcb);
            q_enqueue(createdqueue, pcb2);
            PCB_COUNT += 2;
            break;
        default:
            break;
    }
}

void resetQueue() {

}