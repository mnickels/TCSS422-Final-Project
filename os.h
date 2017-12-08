/*
 * TCSS 422 Final Project
 *
 * Mike Nickels
 * David Foster
 * Yaroslav Salo
 */

#ifndef OS_H
#define OS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "p_queue.h"
#include "timer.h"
#include "iodevice.h"
#include "ourmutex.h"
#include "pcb.h"
#include "mutual_resource.h"

#define IO_PROCESS_LIMIT 50
#define CI_PROCESS_LIMIT 25
#define PC_PAIR_PROCESS_LIMIT 20
#define MRU_PROCESS_LIMIT 20
#define MAX_PCB 115
#define S 100

typedef enum {NO_INTERRUPT = -1, TIMER_INTERRUPT = 0, IO1_INTERRUPT = 1, IO2_INTERRUPT = 2} interrupt_t;
typedef enum {NO_TRAP = -1, IO1_TRAP = 1, IO2_TRAP = 2} trap_t;

void scheduler();
void dispatcher();
void generateInitialPCBs();
void runProcess();
int checkIOInterrupt();
void checkTermination();
void generatePCBs();
void resetQueue();
void pseudo_ISR(interrupt_t);
void scheduler();
void dispatcher();
void addPCB();
void createPCB(enum process_type);
int checkIOTrap();
int checkPCAction(PCB_p);
int checkMRAction(PCB_p);

void deadlock_monitor();

#endif


