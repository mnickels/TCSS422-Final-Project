#include <stdio.h>
#include "pcb.h"

int main() {
	PCB_p p1 = pcb_constructor(IO);
	char * p1_str = pcb_to_string(p1);
	printf("%s\n", p1_str);
	free(p1_str);

	PCB_p p2 = pcb_constructor(IO);
	char * p2_str = pcb_to_string(p2);
	printf("%s\n", p2_str);
	free(p2_str);
	return 0;
}