#ifndef OS_H
#define OS_H

#include "p_queue.h"
#include "timer.h"
#include "iodevice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "ourmutex.h"
#include "pcb.h"

#define IO_PROCESS_LIMIT 50
#define CI_PROCESS_LIMIT 25
#define PC_PAIR_PROCESS_LIMIT 20
#define MRU_PROCESS_LIMIT 20
#define MAX_PCB 115
#define S 50000

typedef enum {TIMER_INTERRUPT = 0, IO1_INTERRUPT = 1, IO2_INTERRUPT} interrupt_t;

void scheduler();
void dispatcher();
void generateInitialPCBs();
void runProcess();
int checkIOInterrupt();
void checkTermination();
void generatePCBs();
void resetQueue();

// int signalTimer();
// int generatePCBs();
void pseudo_ISR(interrupt_t);
void scheduler();
void dispatcher();
void addPCB();
void createPCB(enum process_type);
int checkIOTrap();
int checkPCAction(PCB_p);
int checkMRAction(PCB_p);
// void pseudoIRET();
// void terminate();


#endif


