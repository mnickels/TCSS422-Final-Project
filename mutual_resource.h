#ifndef RESOURCE_H
#define RESOURCE_H 
#include "pcb.h"

typedef struct mut_res {
	PCB_p a;
	PCB_p b;
} mutual_resource_s;

typedef mutual_resource_s * MUT_p;
MUT_p mut_constructor(PCB_p, PCB_p);
void mut_deconstructor(MUT_p mut);
#endif

