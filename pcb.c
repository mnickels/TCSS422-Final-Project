/*
TCSS422
Problem #1
Group Members:
Joshua Meigs
Yaroslav Salo
*/
#include "pcb.h"
unsigned int GLOBAL_PID = 1;
unsigned int PAIR_ID = 1;

/************************************************************
*              CPU_context_s FUNCTION DEFINITIONS           *
*************************************************************/

int check_pointer(void * ptr) {
    if(ptr == NULL || ptr == 0) return NO_OBJ_ERR;
    return NO_ERR;
}

CPU_context_p cpu_context_constructor(void) {
    CPU_context_p context_ptr = (CPU_context_p) malloc(sizeof(CPU_context_s));
    cpu_context_init(context_ptr);
    return context_ptr;
}

int cpu_context_deconstructor(CPU_context_p *context_ptr) {
    if (check_pointer(context_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR;
    free(*context_ptr);
    *context_ptr = NULL; //set the pointer to null for safety.
    return NO_ERR;
}

int cpu_context_init(CPU_context_p context_ptr) {
    if (check_pointer(context_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR;
    for(int i = 0; i < NUM_CONTEXT_REGS; i++) {
        // 1. cast the struct pointer to a char * so we can do pointer arithmetic
        // 2. calculate the offset of every struct member and add that to the pointer
        // 3. cast the char pointer to an unsign int *
        // 4. dereference the pointer and set it to the value.
        ( *(unsigned int *) ( (char *) context_ptr + sizeof(unsigned int ) * i) ) = 0;
    }
    return NO_ERR;
}

unsigned int cpu_context_get_reg(CPU_context_p context_ptr, unsigned int offset) {
    if (check_pointer(context_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR;
    if (offset > sizeof(CPU_context_s)) return OFFSET_ERR;
    return *(unsigned int *)((char *)context_ptr + offset);
}

int cpu_context_set_reg(CPU_context_p context_ptr, unsigned int offset, int value){
    if (check_pointer(context_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR;
    if (offset > sizeof(CPU_context_s)) return OFFSET_ERR;
    *(unsigned int *)((char *)context_ptr + offset) = value;
    return NO_ERR;
}

char * cpu_context_to_string(CPU_context_p context_ptr) {
    //check pointer
    if (check_pointer(context_ptr) == NO_OBJ_ERR) {
        char *err = malloc (sizeof (char));
        err[0] = NO_OBJ_ERR;
        return err;
    }

    char * str = (char *) malloc(sizeof(char) * MAX_CONTEXT_LEN);
    //format the string.
    sprintf(str, "\nPC:  0x%X IR:  0x%X PSR: 0x%X R0:  0x%X R1:  0x%X R2: 0x%X"
        " R3: 0x%X R4: 0x%X R5: 0x%X R6: 0x%X R7: 0x%X\n"
        "**********************************************"
        "***********************************************************\n"
        , context_ptr->pc, context_ptr->ir, context_ptr->psr, context_ptr->r0, context_ptr->r1
        , context_ptr->r2, context_ptr->r3, context_ptr->r4, context_ptr->r5, context_ptr->r6
        , context_ptr->r7
        );
    return str;
}

/************************************************************
*                PCB_s FUNCTION DEFINITIONS                *
*************************************************************/

PCB_p pcb_constructor(enum process_type ptype) {
    PCB_p pcb_ptr = (PCB_p) malloc(sizeof(PCB_s));
    pcb_init(pcb_ptr, ptype);
    return pcb_ptr;
}

int pcb_init(PCB_p pcb_ptr, enum process_type ptype) {
    if (check_pointer(pcb_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR; 
    pcb_ptr->parent = GLOBAL_PID;                   // the parent is the process that came before
    pcb_set_pid(pcb_ptr);
    pcb_ptr->state = new;
    pcb_ptr->priority = 0;                          // all new process get priority 0 
    pcb_ptr->mem = (unsigned char *) pcb_ptr;       // ?????????
    pcb_ptr->size = 0;                              // ????????? 
    pcb_ptr->channel_no = 0;                        // ?????????
    pcb_ptr->context = cpu_context_constructor();   // possibility that malloc returns NULL
    pcb_ptr->creation = clock();
    pcb_ptr->max_pc = rand() % 3000 + 2000;         // values between [2000, 4999]
    pcb_ptr->termination = 0;
    pcb_ptr->terminate = rand() % 20 + 1;           // values between [1, 20]
    pcb_ptr->term_count = 0;
    pcb_ptr->p_type = ptype;
    pcb_ptr->waiting_on_lock = 0;

    init_io_1(pcb_ptr);
    init_io_2(pcb_ptr);


    return NO_ERR;
}

int pcb_deconstructor(PCB_p *pcb_ptr) {
    if (check_pointer(pcb_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR;
    free(*pcb_ptr);
    *pcb_ptr = NULL;
    return NO_ERR;
}

int pcb_set_pid(PCB_p pcb_ptr){
    if (check_pointer(pcb_ptr) == NO_OBJ_ERR) return NO_OBJ_ERR;
    pcb_ptr->pid = GLOBAL_PID++;
    return NO_ERR;
}

int assignPair(PCB_p pcb_p, PCB_p pcb_c) {
    if (check_pointer(pcb_p) == NO_OBJ_ERR || check_pointer(pcb_c) == NO_OBJ_ERR) return NO_OBJ_ERR;
    pcb_p->pair_type = producer;
    pcb_c->pair_type = consumer;
    pcb_p->pair_id = PAIR_ID;
    pcb_c->pair_id = PAIR_ID++;
    unsigned int * shared_resource = calloc(1, sizeof(int));
    MUTEX_p mutex = calloc(1, sizeof(MUTEX_s));
    pcb_c->shared_resource = shared_resource;
    pcb_p->shared_resource = shared_resource;
    pcb_p->mutex = mutex;
    pcb_c->mutex = mutex;
    return NO_ERR;
}

char * pcb_to_string(PCB_p pcb_ptr) {
    //check the pointer
    if (check_pointer(pcb_ptr) == NO_OBJ_ERR) {
        char *err = malloc (sizeof (char));
        err[0] = NO_OBJ_ERR;
        return err;
    }
    //allocate temp memory for the context string,
    char *temp = cpu_context_to_string(pcb_ptr->context);
    char *str = (char *) malloc(sizeof(char) * MAX_PCB_LEN);
    //format the string
    sprintf(str,         "**********************************************"
        "***********************************************************\n"
        "PID: 0x%X STATE: 0x%X PARENT: 0x%X PRIORITY: 0x%X MEM: 0x%p "
        "SIZE: 0x%X CHANNEL_NO: 0x%X"
    , pcb_ptr->pid, pcb_ptr->state, pcb_ptr->parent, pcb_ptr->priority
    , pcb_ptr->mem, pcb_ptr->size, pcb_ptr->channel_no
    );

    //concatenate the context string to the pcb string.
    strcat(str, temp);
    //free the memory of the temporary context string.
    free(temp);
    return str;
}  

void init_io_1(PCB_p my_pcb) {
    for(int i = 0; i < 4; i++) {
        my_pcb->io_1_traps[i] = rand() % 400 + (i * (rand() % 100 + 300));
        //printf("num: %u\n",  my_pcb->io_1_traps[i]);
    }
}

void init_io_2(PCB_p my_pcb) {
    for(int i = 0; i < 4; i++) {
        my_pcb->io_2_traps[i] = rand() % 250 + (i * (rand() % 100 + 250));
        //printf("num: %u\n",  my_pcb->io_2_traps[i]);
    }
}
