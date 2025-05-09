/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 500
#define MAXLINELENGTH 1000
#define MAXFILES 6

static inline void printHexToFile(FILE *, int);

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
	char label[7];
	char location;
	unsigned int offset;
};

struct RelocationTableEntry {
    unsigned int file;
	unsigned int offset;
	char inst[6];
	char label[7];
};

struct FileData {
	unsigned int textSize;
	unsigned int dataSize;
	unsigned int symbolTableSize;
	unsigned int relocationTableSize;
	unsigned int textStartingLine; // in final executable
	unsigned int dataStartingLine; // in final executable
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry symbolTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
	unsigned int textSize;
	unsigned int dataSize;
	unsigned int symbolTableSize;
	unsigned int relocationTableSize;
	int text[MAXSIZE * MAXFILES];
	int data[MAXSIZE * MAXFILES];
	SymbolTableEntry symbolTable[MAXSIZE * MAXFILES];
	RelocationTableEntry relocTable[MAXSIZE * MAXFILES];
};

int main(int argc, char *argv[]) {
	char *inFileStr, *outFileStr;
	FILE *inFilePtr, *outFilePtr; 
	unsigned int i, j;

    if (argc <= 2 || argc > 8 ) {
        printf("error: usage: %s <MAIN-object-file> ... <object-file> ... <output-exe-file>, with at most 5 object files\n",
				argv[0]);
		exit(1);
	}

	outFileStr = argv[argc - 1];

	outFilePtr = fopen(outFileStr, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileStr);
		exit(1);
	}

	FileData files[MAXFILES];

  // read in all files and combine into a "master" file
	for (i = 0; i < argc - 2; ++i) {
		inFileStr = argv[i+1];

		inFilePtr = fopen(inFileStr, "r");
		printf("opening %s\n", inFileStr);

		if (inFilePtr == NULL) {
			printf("error in opening %s\n", inFileStr);
			exit(1);
		}

		char line[MAXLINELENGTH];
		unsigned int textSize, dataSize, symbolTableSize, relocationTableSize;

		// parse first line of file
		fgets(line, MAXSIZE, inFilePtr);
		sscanf(line, "%d %d %d %d",
				&textSize, &dataSize, &symbolTableSize, &relocationTableSize);

		files[i].textSize = textSize;
		files[i].dataSize = dataSize;
		files[i].symbolTableSize = symbolTableSize;
		files[i].relocationTableSize = relocationTableSize;

		// read in text section
		int instr;
		for (j = 0; j < textSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			instr = strtol(line, NULL, 0);
			files[i].text[j] = instr;
		}

		// read in data section
		int data;
		for (j = 0; j < dataSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			data = strtol(line, NULL, 0);
			files[i].data[j] = data;
		}

		// read in the symbol table
		char label[7];
		char type;
		unsigned int addr;
		for (j = 0; j < symbolTableSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%s %c %d",
					label, &type, &addr);
			files[i].symbolTable[j].offset = addr;
			strcpy(files[i].symbolTable[j].label, label);
			files[i].symbolTable[j].location = type;
		}

		// read in relocation table
		char opcode[7];
		for (j = 0; j < relocationTableSize; ++j) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%d %s %s",
					&addr, opcode, label);
			files[i].relocTable[j].offset = addr;
			strcpy(files[i].relocTable[j].inst, opcode);
			strcpy(files[i].relocTable[j].label, label);
			files[i].relocTable[j].file	= i;
		}
		fclose(inFilePtr);
	} // end reading files

	// *** INSERT YOUR CODE BELOW ***
	//    Begin the linking process
	//    Happy coding!!!

	CombinedFiles merged_files; 
	merged_files.dataSize = 0; 
	merged_files.textSize = 0;
	merged_files.relocationTableSize = 0;
	merged_files.symbolTableSize = 0;  

	// setting size variables
	for (int i = 0; i < argc - 2; i++){
		merged_files.textSize += files[i].textSize; 
		merged_files.dataSize += files[i].dataSize; 
		merged_files.symbolTableSize += files[i].symbolTableSize; 
		merged_files.relocationTableSize += files[i].relocationTableSize; 
	}

	//copying over text
	int address = 0; 
	for (int i = 0; i < argc - 2; i++){
		files[i].textStartingLine = address; 
		for (int j = 0; j < files[i].textSize; j++){
			merged_files.text[address] = files[i].text[j]; 
			address++; 
		}
	}

	//copying over data
	address = 0; 
	for (int i = 0; i < argc - 2; i++){
		files[i].dataStartingLine = address; 
		for (int j = 0; j < files[i].dataSize; j++){
			merged_files.data[address] = files[i].data[j]; 
			address++; 
		}
	}
	
	//copying over symbols and getting absolute address
	int num_labels = 0; 
	for (int i = 0; i < argc - 2; i++){
		for (int j = 0; j < files[i].symbolTableSize; j++){
			if (files[i].symbolTable[j].location == 'D'){
				strcpy(merged_files.symbolTable[num_labels].label, files[i].symbolTable[j].label);
				merged_files.symbolTable[num_labels].offset = merged_files.textSize + files[i].dataStartingLine + files[i].symbolTable[j].offset; 
				num_labels++;
			}
			else if (files[i].symbolTable[j].location == 'T'){
				strcpy(merged_files.symbolTable[num_labels].label, files[i].symbolTable[j].label);
				merged_files.symbolTable[num_labels].offset = files[i].textStartingLine + files[i].symbolTable[j].offset;
				num_labels++;
			}
		}
	}

	//error checking duplicate globals
	for (int i = 0; i < num_labels - 1; i++){
		for (int j = i+1; j < num_labels; j++){
			if (!strcmp(merged_files.symbolTable[i].label, merged_files.symbolTable[j].label)){
				printf("Duplicate defined global labels"); 
				exit(1); 
			}
		}
	}
	// error checking for stack label
	for (int i = 0; i < num_labels; i++){
		if (!strcmp(merged_files.symbolTable[i].label, "Stack")){
			printf("Using reserved label Stack");
			exit(1); 
		}
	}
	// error checking undefined globals
	for (int i = 0; i < argc - 2; i++){
		for (int j = 0; j < files[i].symbolTableSize; j++){
			if (files[i].symbolTable[j].location == 'U' && strcmp(files[i].symbolTable[j].label, "Stack")){
				int found = 0; 
				for (int k = 0; k < num_labels; k++){
					if (!strcmp(merged_files.symbolTable[k].label, files[i].symbolTable[j].label)){
						found = 1; 
						break; 
					}
				}
				if (!found){
					printf("undefined global label");
					exit(1);
				}
			}
		}
	}

	// resolving instructions via relocation table
	for (int i = 0; i < argc - 2; i++){
		for (int j = 0; j < files[i].relocationTableSize; j++){
			int offset = 0; 
			if (!strcmp(files[i].relocTable[j].inst, "lw") || !strcmp(files[i].relocTable[j].inst, "sw")){
				offset = files[i].textStartingLine + files[i].relocTable[j].offset; 
			}
			else if (!strcmp(files[i].relocTable[j].inst, ".fill")){
				offset = files[i].dataStartingLine + files[i].relocTable[j].offset; 
			}
			// if its Stack label 
			if (!strcmp(files[i].relocTable[j].label, "Stack")){
				if (!strcmp(files[i].relocTable[j].inst, "lw") || !strcmp(files[i].relocTable[j].inst, "sw")){
					merged_files.text[offset] = (merged_files.text[offset] & 0xFFFF0000) + merged_files.textSize + merged_files.dataSize;
				}
				else if (!strcmp(files[i].relocTable[j].inst, ".fill")){
					merged_files.data[offset] = merged_files.textSize + merged_files.dataSize;
				}
			}
			// global label
			else if (files[i].relocTable[j].label[0] >= 'A' && files[i].relocTable[j].label[0] <= 'Z'){
				for (int k = 0; k < merged_files.symbolTableSize; k++){
					if (!strcmp(merged_files.symbolTable[k].label, files[i].relocTable[j].label)){
						if (!strcmp(files[i].relocTable[j].inst, "lw") || !strcmp(files[i].relocTable[j].inst, "sw")){
							merged_files.text[offset] = (merged_files.text[offset] & 0xFFFF0000) + merged_files.symbolTable[k].offset;
							break; 
						}
						else if (!strcmp(files[i].relocTable[j].inst, ".fill")){
							merged_files.data[offset] = merged_files.symbolTable[k].offset;
							break; 							
						}
					}
				}
			}
			// local label
			else {
				if (!strcmp(files[i].relocTable[j].inst, "lw") || !strcmp(files[i].relocTable[j].inst, "sw")){
					// sw or lw from data
					if ((merged_files.text[offset] & 0xFFFF) >= files[i].textSize){
						merged_files.text[offset] += merged_files.textSize - files[i].textSize + files[i].dataStartingLine;
					}
					else {
						merged_files.text[offset] += files[i].textStartingLine;
					}
				}
				else if (!strcmp(files[i].relocTable[j].inst, ".fill")){
					if (merged_files.data[offset] >= files[i].textSize){
						merged_files.data[offset] += merged_files.textSize - files[i].textSize + files[i].dataStartingLine; 
					}
					else {
						merged_files.data[offset] += files[i].textStartingLine;
					}
				}
			}
		}
	}

	//final output
	for (int i = 0; i < merged_files.textSize; i++){
		printHexToFile(outFilePtr, merged_files.text[i]); 
	}
	for (int i = 0; i < merged_files.dataSize; i++){
		printHexToFile(outFilePtr, merged_files.data[i]); 
	}	

} // main

// Prints a machine code word in the proper hex format to the file
static inline void 
printHexToFile(FILE *outFilePtr, int word) {
    fprintf(outFilePtr, "0x%08X\n", word);
}
