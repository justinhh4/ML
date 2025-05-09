/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define MEMORYSIZE 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[MEMORYSIZE];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

static inline int convertNum(int32_t);
void add(stateType * ); 
void nor(stateType * ); 
void lw(stateType * ); 
void sw(stateType * ); 
void beq(stateType * ); 
void jalr(stateType * ); 
void noop(stateType * ); 

int 
main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(2);
    }


    /* read the entire machine-code file into memory */
    for (state.numMemory=0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
		    if (state.numMemory >= MEMORYSIZE) {
			      fprintf(stderr, "exceeded memory size\n");
			      exit(2);
		    }
		    if (sscanf(line, "%x", state.mem+state.numMemory) != 1) {
			      fprintf(stderr, "error in reading address %d\n", state.numMemory);
			      exit(2);
		    }
		    printf("memory[%d]=0x%08X\n", state.numMemory, state.mem[state.numMemory]);
    }
    for (int i = 0; i < NUMREGS; i++){
        state.reg[i] = 0; 
    }

    printState(&state);
    
    
    while (state.pc < state.numMemory){
        int opcode = state.mem[state.pc] >> 22; 
        if (opcode == 0){ // add
            add(&state);
        }
        else if (opcode == 1){  // nor
            nor(&state); 
        }
        else if (opcode == 2){ // lw
            lw(&state); 
        }
        else if (opcode == 3){ // sw
            sw(&state); 
        }
        else if (opcode == 4){ // beq
            beq(&state); 
        }
        else if (opcode == 5){ //jalr
            jalr(&state); 
        }
        else if (opcode == 6){  //halt
            state.pc++; 
            printState(&state); 
            break; 
        }
        else if (opcode == 7){  //noop
            noop(&state); 
        }
        else {
            printf("unrecognized opcode ");
            exit(1); 
        }
        printState(&state); 
    }

    //Your code ends here! 

    return(0);
}

void add(stateType *statePtr){
    int code = statePtr->mem[statePtr->pc]; 
    int r1 = (code >> 19) & 0b0111; 
    int r2 = (code >> 16) & 0b0111;
    int r3 = code & 0b0111; 
    statePtr->reg[r3] = statePtr->reg[r2] + statePtr->reg[r1]; 
    statePtr->pc++; 
}

void nor(stateType *statePtr){
    int code = statePtr->mem[statePtr->pc]; 
    int r1 = (code >> 19) & 0b0111; 
    int r2 = (code >> 16) & 0b0111;
    int r3 = code & 0b0111; 
    statePtr->reg[r3] = ~(statePtr->reg[r2] | statePtr->reg[r1]); 
    statePtr->pc++; 
}

void lw(stateType *statePtr){
    int code = statePtr->mem[statePtr->pc]; 
    int r1 = (code >> 19) & 0b0111; 
    int r2 = (code >> 16) & 0b0111;
    int offsetField = convertNum(code & 0xFFFF); 
    statePtr->reg[r2] = statePtr->mem[offsetField + statePtr->reg[r1]];
    statePtr->pc++; 
}

void sw(stateType *statePtr){
    int code = statePtr->mem[statePtr->pc]; 
    int r1 = (code >> 19) & 0b0111; 
    int r2 = (code >> 16) & 0b0111;
    int offsetField = convertNum(code & 0xFFFF); 
    statePtr->mem[offsetField + statePtr->reg[r1]] = statePtr->reg[r2]; 
    statePtr->pc++; 
}

void beq(stateType *statePtr){
    int code = statePtr->mem[statePtr->pc]; 
    int r1 = (code >> 19) & 0b0111; 
    int r2 = (code >> 16) & 0b0111;
    int offsetField = convertNum(code & 0xFFFF); 
    if (statePtr->reg[r1] == statePtr->reg[r2]){
        statePtr->pc = statePtr->pc + offsetField + 1; 
    }
    else {
        statePtr->pc++;
    }
}

void jalr(stateType * statePtr){
    int code = statePtr->mem[statePtr->pc]; 
    int r1 = (code >> 19) & 0b0111; 
    int r2 = (code >> 16) & 0b0111;
    statePtr->reg[r2] = statePtr->pc + 1; 
    statePtr->pc = statePtr->reg[r1]; 
}

void noop(stateType * statePtr){
    statePtr->pc++;
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void printState(stateType *statePtr) {
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] 0x%08X\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
	  for (i=0; i<NUMREGS; i++) {
	      printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	  }
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/
