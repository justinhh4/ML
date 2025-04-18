/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
static inline void printHexToFile(FILE *, int);


int check_register(char *reg){
    if (!isNumber(reg)){
        printf("register is not an integer");
        exit(1); 
    }
    int r = atoi(reg); 
    if (r < 0 || r > 7){
        printf("register not between 0 and 7");
        exit(1); 
    }
    return r; 
}



struct Label{
    char label[7];
    int address; 
};

int
main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    struct Label labels[MAXLINELENGTH]; 
    for (int i = 0; i < MAXLINELENGTH; i++){
        labels[i].label[0] = '\0'; 
        labels[i].address = -1; 
    }


    //////  first pass  /////
    int label_counter = 0; 
    int memory_number = 0; 
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        // if line has label
        if (strlen(label) != 0 && !isNumber(label)){  
            bool found_label = false; 
            // find label if already in array 
            for (int i = 0; i < label_counter; i++){
                if (!strcmp(labels[i].label, label)){
                    if (labels[i].address != -1){
                        printf("duplicate label "); 
                        exit(1);
                    }
                    found_label = true; 
                    labels[i].address = memory_number; 
                    break; 
                }
            }
            // create label if not found 
            if (!found_label){
                strcpy(labels[label_counter].label, label);
                labels[label_counter].address = memory_number; 
                label_counter++;
            }
        }
        // if arg2 has label 
        if ((!strcmp(opcode , "lw") || !strcmp(opcode , "sw") || !strcmp(opcode , "beq")) && strlen(arg2) != 0 && !isNumber(arg2)){
            bool found_label = false; 
            // find if already in the array
            for (int i = 0; i < label_counter; i++){
                if (!strcmp(labels[i].label, arg2)){
                    found_label = true; 
                    break; 
                }
            }
            if (!found_label){
                strcpy(labels[label_counter].label, arg2);
                label_counter++;
            }                   
        }
        // if opcode is .fill and arg0 is label
        if (!strcmp(opcode, ".fill") && !isNumber(arg0)){
            bool found_label = false; 
            // find if already in the array
            for (int i = 0; i < label_counter; i++){
                if (!strcmp(labels[i].label, arg0)){
                    found_label = true; 
                    break; 
                }
            }
            if (!found_label){
                strcpy(labels[label_counter].label, arg0);
                label_counter++;
            }                   
        }        
        memory_number++; 
    }
    for (int i = 0; i < label_counter; i++){
        if (strlen(labels[i].label) != 0 && labels[i].address == -1){
            printf("Use of undefined label");
            exit(1); 
        }
    }

    rewind(inFilePtr); 
    // second pass 
    int hex_array[MAXLINELENGTH]; 
    memory_number = 0;
    
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        int code = 0; 
        if (!strcmp(opcode , "add")){
            code = 0b0000000000; 
            int r1 = check_register(arg0); 
            int r2 = check_register(arg1);
            int r3 = check_register(arg2);
            code = code << 3; 
            code += r1; 
            code = code << 3; 
            code += r2; 
            code = code << 16;
            code += r3; 
        }
        else if (!strcmp(opcode , "nor")){
            code = 0b0000000001; 
            int r1 = check_register(arg0); 
            int r2 = check_register(arg1);
            int r3 = check_register(arg2);
            code = code << 3; 
            code += r1; 
            code = code << 3; 
            code += r2; 
            code = code << 16;
            code += r3;  

        }
        else if (!strcmp(opcode , "lw")){
            code = 0b0000000010; 
            int r1 = check_register(arg0); 
            int r2 = check_register(arg1); 
            code = code << 3; 
            code += r1; 
            code = code << 3; 
            code += r2; 
            code = code << 16; 
            if (isNumber(arg2)){
                int arg = atoi(arg2); 
                if (arg > 32767 || arg < -32768){
                    printf("offsetField integer overflow"); 
                    exit(1); 
                }
                else {
                    code += arg & 0xFFFF; 
                }
            }
            else {
                for (int i = 0; i < label_counter; i++){
                    if (!strcmp(labels[i].label, arg2)){
                        code += labels[i].address & 0xFFFF; 
                        break; 
                    }
                }                
            }
        }
        else if (!strcmp(opcode , "sw")){
            code = 0b0000000011; 
            int r1 = check_register(arg0); 
            int r2 = check_register(arg1); 
            code = code << 3; 
            code += r1; 
            code = code << 3; 
            code += r2; 
            code = code << 16; 
            if (isNumber(arg2)){
                int arg = atoi(arg2); 
                if (arg > 32767 || arg < -32768){
                    printf("offsetField integer overflow"); 
                    exit(1); 
                }
                else {
                    code += arg & 0xFFFF; 
                }
            }
            else {
                for (int i = 0; i < label_counter; i++){
                    if (!strcmp(labels[i].label, arg2)){
                        code += labels[i].address & 0xFFFF; 
                        break; 
                    }
                }                
            }
        }            
        else if (!strcmp(opcode , "beq")){
            code = 0b0000000100; 
            int r1 = check_register(arg0); 
            int r2 = check_register(arg1); 
            code = code << 3; 
            code += r1; 
            code = code << 3; 
            code += r2; 
            code = code << 16; 
            if (isNumber(arg2)){
                int arg = atoi(arg2); 
                if (arg > 32767 || arg < -32768){
                    printf("offsetField integer overflow"); 
                    exit(1); 
                }
                else {
                    code += arg & 0xFFFF; 
                }
            }
            else {
                for (int i = 0; i < label_counter; i++){
                    if (!strcmp(labels[i].label, arg2)){
                        code += ((labels[i].address - memory_number - 1) & 0xFFFF); 
                        break; 
                    }
                }                
            }            
        }
        else if (!strcmp(opcode , "jalr")){
            code = 0b0000000101; 
            int r1 = check_register(arg0); 
            int r2 = check_register(arg1); 
            code = code << 3; 
            code += r1; 
            code = code << 3; 
            code += r2; 
            code = code << 16;             
        }
        else if (!strcmp(opcode , "halt")){
            code = 0b00000001100000000000000000000000;
        }
        else if (!strcmp(opcode , "noop")){
            code = 0b00000001110000000000000000000000;
        }
        else if (!strcmp(opcode, ".fill")){
            if (isNumber(arg0)){
                code = atoi(arg0);
            }
            else {
                for (int i = 0; i < label_counter; i++){
                    if (!strcmp(labels[i].label, arg0)){
                        code = labels[i].address; 
                        break; 
                    }
                }                  
            }
        }
        else {
            printf("unrecgonized opcode");
            exit(1); 
        }
        hex_array[memory_number] = code; 
        memory_number++; 
    }
    for (int i = 0; i < memory_number; i++){
        printHexToFile(outFilePtr, hex_array[i]); 
    }


    return(0);
}

// Returns non-zero if the line contains only whitespace.
static int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr) {
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for(int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address) {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH-1) {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if(lineIsBlank(line)) {
            if(!blank_line_encountered) {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        } else {
            if(blank_line_encountered) {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}


/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}


// Prints a machine code word in the proper hex format to the file
static inline void 
printHexToFile(FILE *outFilePtr, int word) {
    fprintf(outFilePtr, "0x%08X\n", word);
}
