#ifndef OS_H
#define OS_H

#include "p_queue.h"
#include "ourthread.h"
#include "timer.h"
#include "iodevice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IO_PROCESS_LIMIT 50
#define CI_PROCESS_LIMIT 25
#define PC_PAIR_PROCESS_LIMIT 10
#define MRU_PROCESS_LIMIT 10
#define MAX_PCB 10
#define S 50000

void scheduler();
void dispatcher();
void generateInitialPCBs();
void runProcess();
int checkIOInterrupt();
int checkTermination();
void generatePCBs();
void resetQueue();

// int signalTimer();
// int generatePCBs();
// void timer_pseudoISR();
// void IO_pseudoISR();
// void scheduler();
// void dispatcher();
// void pseudoIRET();
// void terminate();


#endif


