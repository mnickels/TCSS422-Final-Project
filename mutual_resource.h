#ifndef 
#define RESOURCE_H value
#include "pcb.h"


typedef struct mut_res {
	PCB_p a;
	PCB_p b;
} mutual_resource;
typedef mutual_resource * MUT_p;

MUT_p mut_constructor();
void mut_deconstructor(MUT_p mut);
#endif

