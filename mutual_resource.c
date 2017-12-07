#include "mutual_resource.h"
MUT_p mut_constructor(PCB_p a , PCB_p b) {
	MUT_p mut = calloc(1, sizeof(mutual_resource_s));
	mut->a = a;
	mut->b = b;
	return mut;
}

void mut_deconstructor(MUT_p mut) {
	free(mut);
}