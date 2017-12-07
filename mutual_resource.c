#include "mutual_resource.h"
MUT_p mut_constructor(PCB_pa , PCB_p b) {
	MUT_p mut = calloc(1, sizeof(timer_s));
	mut->a = a;
	mut->b = b;
	return mut;
}

void mut_deconstructor(MUT_p mut) {
	free(mut);
}