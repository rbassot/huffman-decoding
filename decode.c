#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FRENCH_ALPHABET_SIZE 35



void build_look_up_table(char* table, FILE* file){

    char line[256];

    //get first line of file, which dictates the min & max code length
    fgets(line, sizeof(line), file);
    int min_code_len = (int)line[0];
    int max_code_line = (int)line[2];

    //TODO

    //read rest of file line by line
    while (fgets(line, sizeof(line), file)){

    }
    return;
}

//Main function to drive Huffman decoding of n encoded string
//Argument: LUT.txt file that contains a list of character/code pairs
int main(int argc, char *argv[]){

    if(argc!=2) {
		fprintf(stderr, "Argument Error: Expected ./huffman LUT.txt\n");
		return 1;
	}

    //open LUT text file to get character/code pairs for LUT data structure
    FILE *file = fopen(argv[2], "r");
    flcose(file);
 
    //array of size 35 - array indices are ordered French characters (1-35), array values are the associated codes for each character
    //Notice: this is a char array, so the max code length is 8 bits here
    uint8_t look_up_table[FRENCH_ALPHABET_SIZE] = { 0 };

    //inititalize the lookup table with
    build_look_up_table(look_up_table, file);
    return 0;
}