/*
init: create initial processes, create 2 io threads and timer thread
quantum = (20*priority)+50
globals = readyqueue, currentProcess, S= 50,000
processes = 50 IO, 25 compute intensive, 10 pc pairs, 10 mutual resource
for(;;)
    run process() / pc++
    check io arrays()
    checktermination()
    generateProcesses()
    if cpu->pc%S == 0
        resetQueue()

scheduler()
dispatcher() starts timer
timer()        

*/
#include "os.h"

// #define S = 50000
// #define MAX_IO_PCB = 50
// #define MAX_CI_PCB = 25
// #define MAX_PC_PCB = 10
// #define MAX_MR_PCB = 10
// #define MAX_PCB = 10;

P_QUEUE_p readyqueue;
DEVICE_p IOdevice1;
DEVICE_p IOdevice2;
PCB_p currentprocess;
unsigned int PC;
int PCB_COUNT = 0;

int main() {
    srand(0);
    readyqueue = p_q_constructor();
    IOdevice1 = device_constructor(DEVICE_1);
    IOdevice2 = device_constructor(DEVICE_2);
    //initialize timer
    PC = 0;
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
        generatePCBs();
        if(PC == S){
            resetQueue();
        }
        if (PC == 1000) break;
    }
}

void scheduler() {

}

void dispatcher() {

}

void generateInitialPCBs() {
    for(int i = 0; i < 3; i++){
        PCB_p pcb = pcb_constructor();
        p_q_enqueue(readyqueue, pcb);
        PCB_COUNT++;
    }
    currentprocess = p_q_dequeue(readyqueue);
    currentprocess->state = ready;
}

void runProcess() {
    PC++;
    //check process type and take appropriate action
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

void generatePCBs() {
    int toAdd = rand() % 5;
    for(int i = 0; i < toAdd; i++){
        if(PCB_COUNT < MAX_PCB){
            PCB_p pcb = pcb_constructor();
            p_q_enqueue(readyqueue, pcb);
            PCB_COUNT++;
        }
    }
}

void resetQueue() {

}