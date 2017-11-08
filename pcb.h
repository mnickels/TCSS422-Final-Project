#ifndef PCB_BLOCK
#define PCB_BLOCK
#define NO_ERR 0									// Indicates no error. 
#define NO_OBJ_ERR -1								// Indicates an error.
#define OFFSET_ERR -2 								// Indicates that the offest is larger than the struct size.
#define NUM_CONTEXT_REGS 11							// Number of registers in the context.
#define MAX_PCB_LEN 500								// Max length pcb representation string can be.
#define MAX_CONTEXT_LEN 300							// Max length cpu_context representation string can be.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

/* 
* The process identification number.
*/ 
extern unsigned int GLOBAL_PID;

/*
* CPU state for the LC-3 processor.
*/
typedef struct cpu_context {
	unsigned int pc;
	unsigned int ir;
	unsigned int psr;
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
} CPU_context_s;

/* 
* Define a pointer to the CPU_context_s. 
*/
typedef CPU_context_s * CPU_context_p;

/*
* Possible states of the pcb.
*/
enum state_type {new, ready, running, interrupted, waiting, halted};

/*
* Process control block struct. 
*/
typedef struct pcb { 							    
	unsigned int pid; 							    // process identification.
	enum state_type state; 						    // process state. 
	unsigned int parent; 						    // parent process pid.
	unsigned char priority; 					    // 0 is highest – 15 is lowest.
	unsigned char * mem; 						    // start of process in memory.
	unsigned int size; 							    // number of bytes in process.
	unsigned char channel_no; 					    // which I/O device or service.
	// if process is blocked, which queue it is in
	CPU_context_p context;                          // set of cpu registers
    
    // other items to be added as needed. 
} PCB_s;											

/* 
* Define a pointer to the PCB_s. 
*/
typedef PCB_s * PCB_p;


/**********************************************
*  CPU_context_s MEMBER FUNCTIONS PROTOTYPES  *
***********************************************/

/* 
* Given a pointer checks if it is valid. 
* returns -1 if it is and 0 if it is not.
*/
int check_pointer(void * ptr);

/*
* Return the contents of the register indicated by the offset from the context_ptr. 
* To compute the offset use offsetof(Struct_Name, Desired_Struct_Member_Name) macro.
* If context_ptr is invalid returns MAX(unsigned int) which can be interpreted as int = -1, otherwise 0.
*/
unsigned int cpu_context_get_reg(CPU_context_p context_ptr, unsigned int offset);

/*
* Set the contents of the register indicated by the offset from the context_ptr to the passed in value. 
* To compute the offset use offsetof(Struct_Name, Desired_Struct_Member_Name) macro.
* If context_ptr is invalid returns a -1 otherwise 0.
*/
int cpu_context_set_reg(CPU_context_p context_ptr, unsigned int offset, int value);

/*
* Allocates memory for CPU_context_p struct and returns a pointer to it.
*/
CPU_context_p cpu_context_constructor(void);

/*
* Free the memory pointed to by the *context_ptr and set that 
* pointer to NULL. Because a pointer is pass by value the we need a **.
* If *context_ptr is invalid returns a -1 otherwise 0.
*/
int cpu_context_deconstructor(CPU_context_p  *context_ptr);

/*
* Initialize cpu context registers to 0.
* If context_ptr is invalid returns a -1 otherwise 0.
*/
int cpu_context_init(CPU_context_p context_ptr);

/*
* Write the contentens of this CONTEXT as a string to the 
* memory location provided by the caller in str.
* The caller should allocate/free the required memory.
* Returs -1 if either of the pointers were invalid otherwise returns 0.  
*/
char * cpu_context_to_string(CPU_context_p context_ptr);

/**********************************************
*       PCB_s MEMBER FUNCTIONS PROTOTYPES     *
***********************************************/

/*
* Assigns a PID to the process and increments the global variable by 1.
*/
PCB_p pcb_constructor(void);

/*
* Free the memory pointed to by the *pcb_ptr and set that 
* pointer to NULL. Because a pointer is pass by value the we need a **.
* If *pcb_ptr is invalid returns a NO_OBJ_ERR otherwise NO_ERR.
*/
int pcb_deconstructor(PCB_p *pcb_ptr);

/*
* Sets the pid of this PCB and increments the GLOBAL_PID by one. 
*/
int pcb_set_pid(PCB_p pcb_ptr);

/*
* Initializes the pcb members. 
* If pcb_ptr is invalid returns a -1 otherwise 0.
*/
int pcb_init(PCB_p pcb_ptr);

/*
* Write the contentens of this PCB as a string to the 
* memory location provided by the caller in str.
* The caller should allocate/free the required memory.
* Returs -1 if either of the pointers were invalid otherwise returns 0. 
*/
char * pcb_to_string(PCB_p pcb_ptr);

#endif 