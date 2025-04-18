/**
 * Project 2
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

/**
 * Requires: readAndParse is non-static and unmodified from project 1a. 
 *   inFilePtr and outFilePtr must be opened. 
 *   inFilePtr must be rewound before calling this function.
 * Modifies: outFilePtr
 * Effects: Prints the correct machine code for the input file. After 
 *   reading and parsing through inFilePtr, the pointer is rewound.
 *   Most project 1a error checks are done. No undefined labels of any
 *   type are checked, and these are instead resolved to 0.
*/
/**
 * This function will be provided in an instructor object file once the
 * project 1a deadline + late days has passed.
*/
// extern void print_inst_machine_code(FILE *inFilePtr, FILE *outFilePtr);

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
    bool is_global; 
    char global_type; 
};

int main(int argc, char **argv) {
    char *inFileStr, *outFileStr;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileStr = argv[1];
    outFileStr = argv[2];

    inFilePtr = fopen(inFileStr, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileStr);
        exit(1);
    }
    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);
    
    outFilePtr = fopen(outFileStr, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileStr);
        exit(1);
    }

    // implementation here
    int address = 0;
    int num_labels = 0; 
    int num_globals = 0; 
    struct Label labels[MAXLINELENGTH]; 

    // initialize labels
    // potential issue: char array default initialization
    while ( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        if (strlen(label)) {
            // check duplicate labels
            for (int i = 0; i < num_labels; i++){
                if (!strcmp(labels[i].label, label)){
                    printf("duplicate label");
                    exit(1); 
                }
            }
            labels[num_labels].address = address;
            strcpy(labels[num_labels].label, label);
            // check global
            if (label[0] - 'A' >= 0 && label[0] - 'A' <= 25){
                num_globals++; 
                labels[num_labels].is_global = true; 
                if (!strcmp(opcode, ".fill")){
                    labels[num_labels].global_type = 'D'; 
                }
                else {
                    labels[num_labels].global_type = 'T'; 
                }
            }
            else {
                labels[num_labels].is_global = false; 
            }
            num_labels++;
        }
        address++;
    }

    // text
    rewind(inFilePtr);
    int num_text = 0; 
    int num_data = 0; 
    int num_relocations = 0; 
    int text[MAXLINELENGTH]; 
    
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
                num_relocations++; 
                bool found_local_label = false; 
                for (int i = 0; i < num_labels; i++){
                    if (!strcmp(labels[i].label, arg2)){
                        code += labels[i].address & 0xFFFF; 
                        found_local_label = true; 
                        break; 
                    }
                }    
                if (!found_local_label){
                    if (arg2[0] - 'A' >= 0 && arg2[0] - 'A' <= 25){
                        strcpy(labels[num_labels].label, arg2); 
                        labels[num_labels].address = 0; 
                        labels[num_labels].is_global = true; 
                        labels[num_labels].global_type = 'U'; 
                        num_globals++; 
                        num_labels++; 
                    }
                    else {
                        printf("lw use of unitialized label");
                        exit(1); 
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
                num_relocations++; 
                bool found_local_label = false; 
                for (int i = 0; i < num_labels; i++){
                    if (!strcmp(labels[i].label, arg2)){
                        code += labels[i].address & 0xFFFF; 
                        found_local_label = true; 
                        break; 
                    }
                }  
                if (!found_local_label){
                    if (arg2[0] - 'A' >= 0 && arg2[0] - 'A' <= 25){
                        strcpy(labels[num_labels].label, arg2); 
                        labels[num_labels].address = 0; 
                        labels[num_labels].is_global = true; 
                        labels[num_labels].global_type = 'U'; 
                        num_globals++; 
                        num_labels++; 
                    }
                    else {
                        printf("sw use of unitialized label");
                        exit(1); 
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
                bool found_label = false; 
                for (int i = 0; i < num_labels; i++){
                    if (!strcmp(labels[i].label, arg2)){
                        code += ((labels[i].address - num_text - 1) & 0xFFFF); 
                        found_label = true; 
                        break; 
                    }
                }                
                if (!found_label){
                    printf("beq use of undefined label");
                    exit(1); 
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
            num_data++; 
            if (isNumber(arg0)){
                code = atoi(arg0);
            }
            else {
                num_relocations++; 
                bool found_label = false; 
                for (int i = 0; i < num_labels; i++){
                    if (!strcmp(labels[i].label, arg0)){
                        code = labels[i].address; 
                        found_label = true; 
                        break; 
                    }
                }        
                if (!found_label){
                    if (arg0[0] - 'A' >= 0 && arg0[0] - 'A' <= 25){
                        strcpy(labels[num_labels].label, arg0); 
                        labels[num_labels].address = 0; 
                        labels[num_labels].is_global = true; 
                        labels[num_labels].global_type = 'U'; 
                        num_globals++; 
                        num_labels++; 
                    }
                    else {
                        printf(".fill use of unitialized label");
                        exit(1); 
                    }   
                }          
            }
        }
        else {
            printf("unrecgonized opcode");
            exit(1); 
        }
        text[num_text] = code; 
        num_text++; 
    }
    
    // resolve data addresses for symbol table
    for (int i = 0; i < num_labels; i++){
        if (labels[i].is_global && labels[i].global_type == 'D'){
            labels[i].address = labels[i].address - (num_text - num_data); 
        }
    }

    //output
    // header 
    fprintf(outFilePtr, "%d %d %d %d\n", num_text - num_data, num_data, num_globals, num_relocations);

    // text and data
    for (int i = 0; i < num_text; i++){
        printHexToFile(outFilePtr, text[i]); 
    }

    // symbol table 
    for (int i = 0; i < num_labels; i++){
        if (labels[i].is_global){
            fprintf(outFilePtr, "%s %c %d\n", labels[i].label, labels[i].global_type, labels[i].address); 
        }
    }

    // relocation
    rewind(inFilePtr); 
    int current_pos = 0; 
    while ( readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ){
        if (!strcmp(opcode, "sw") || !strcmp(opcode, "lw")){
            if (!isNumber(arg2)){
                fprintf(outFilePtr, "%d %s %s\n", current_pos, opcode, arg2); 
            }
        }
        else if (!strcmp(opcode, ".fill")){
            if(!isNumber(arg0)){
                fprintf(outFilePtr, "%d %s %s\n", current_pos - (num_text - num_data), opcode, arg0); 
            }
        }
        current_pos++; 
    } 
}

/*
* NOTE: The code defined below is not to be modifed as it is implemented correctly.
*/

// Returns non-zero if the line contains only whitespace.
int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++whitespace_idx) {
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
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 */
int readAndParse(FILE *inFilePtr, char *label,
	char *opcode, char *arg0, char *arg1, char *arg2) {

    char line[MAXLINELENGTH];
    char *ptr = line;

    // delete prior values
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    // read the line from the assembly-language file
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		// reached end of file
        return(0);
    }

    // check for line too long
    if (strlen(line) >= MAXLINELENGTH-1) {
		printf("error: line too long\n");
		exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    // is there a label?
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
		// successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    // Parse the rest of the line.  Would be nice to have real regular expressions, but scanf will suffice.
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
