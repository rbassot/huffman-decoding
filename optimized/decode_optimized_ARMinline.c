#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>

/*
* REQUIRED SETUP STEPS:
*   1 - Set HWIDTH to a reasonable value to keep the main LUT to a reasonable size
*   2 - Set MAXWIDTH to the maximum length Huffman code existing in LUT.txt. This must be known beforehand
*   3 - In build_lookup_table() & GLOBALS, change main LUT & extended LUT types to handle respective code values that
*           are seen (i.e. uint8_t & uint_16t are currently used). All instances of these type declarations must be changed.
*/

#define FRENCH_ALPHABET_SIZE 35
#define HWIDTH 6 //This dictates the largest index size of the first LUT - any Huffman Codes longer will be handled
                      //through a pointer to the extended LUT
                      //**As long as this power is less than 8, a LUT of type uint8_t can be used!**

#define MAXWIDTH 8 //This dictates the largest index size of the extended LUT - The power is equivalent to
                        //the length of the longest existing Huffman code
                        //current max power can be 16 (restricted by uint16_t)

#define LUT_SIZE 1 << HWIDTH
#define EXT_LUT_SIZE 1 << MAXWIDTH


/*-----GLOBAL DATA STRUCTURES-----*/
//Main LUT
uint8_t LUT[LUT_SIZE][2] = { 0 };

//Extended LUT
uint16_t extended_LUT[EXT_LUT_SIZE][2] = { 0 };


void build_lookup_tables(FILE* file){

    unsigned int i; //to avoid C99 compile flag, this will have to be optimized
    char line[256];
    unsigned short min_code_len;
    unsigned short max_code_len;
    unsigned short huff_letter;
    char huff_code[MAXWIDTH + 1];
    uint8_t num_huff_code8;
    uint16_t num_huff_code16;
    unsigned int bitmask;

    register char code_len; //heavily reassigned variable per line
    char* end_ptr;

    //get first line of file, which dictates the min & max code length
    fgets(line, sizeof(line), file);
    sscanf(line, "%hu,%hu", &min_code_len, 
            &max_code_len);

    //read rest of file line by line
    while(fgets(line, sizeof(line), file)){
        //get values
        sscanf(line, "%hu,%s", &huff_letter, huff_code);

        //get length of current Huffman code - for storage in the LUT
        code_len = strlen(huff_code);

        //Case 1 - slot character & code length pair into Main LUT
        if(code_len <= HWIDTH){
            //convert string code (binary representation) to number
            //then, left shift the huffcode (LUT index) until it is left aligned
            num_huff_code8 = (uint8_t)strtol(huff_code, &end_ptr, 2) << (HWIDTH - code_len);

            //slot character & code length into the table in all indices that have the given binary prefix
            bitmask = ((1 << HWIDTH) - 1) ^ ((1 << (HWIDTH - code_len)) - 1);
            
            /* LOOP UNROLLING applied - always works as our tables have an even number of indices (size is power of 2) */
            /* Cache Optimized LUT - accessing indices where first dimension of LUt is the largest dimension */
            for(i = 0; i < LUT_SIZE; i += 2){

                if(((i & bitmask) ^ (num_huff_code8 & bitmask)) == 0){
                    LUT[i][0] = huff_letter;
                    LUT[i][1] = code_len;
                }
                if(((i + 1 & bitmask) ^ (num_huff_code8 & bitmask)) == 0){
                    LUT[i + 1][0] = huff_letter;
                    LUT[i + 1][1] = code_len;
                }
            }
        }

        //Case 2 - slot character & code length pair into Extended LUT
        else{
            //convert string code (binary representation) to number
            //then, left shift the huffcode (LUT index) until it is left aligned
            num_huff_code16 = (uint16_t)strtol(huff_code, &end_ptr, 2) << (MAXWIDTH - code_len);

            //slot character & code length into the table in all indices that have the given binary prefix
            bitmask = ((1 << MAXWIDTH) - 1) ^ ((1 << (MAXWIDTH - code_len)) - 1);
            
            /* LOOP UNROLLING applied - always works as our tables have an even number of indices (size is power of 2) */
            /* Cache Optimized LUT - accessing indices where first dimension of LUt is the largest dimension */
            for(i = 0; i < EXT_LUT_SIZE; i += 2){

                if(((i & bitmask) ^ (num_huff_code16 & bitmask)) == 0){
                    extended_LUT[i][0] = huff_letter;
                    extended_LUT[i][1] = code_len;
                }
                if(((i + 1 & bitmask) ^ (num_huff_code16 & bitmask)) == 0){
                    extended_LUT[i + 1][0] = huff_letter;
                    extended_LUT[i + 1][1] = code_len;
                }
            }
        }
    }

    //iterate through main LUT to fill in pointer to extended LUT, wherever there currently exists no index
    //main LUT filling
    unsigned int prev_letter;
    unsigned int prev_code_len;
    unsigned int prev_code;

    /* LOOP UNROLLING applied */
    for(i = 0; i < LUT_SIZE; i += 2){

        if(LUT[i][1] == 0){
            
            LUT[i][0] = '&';
        }
        if(LUT[i + 1][1] == 0){
            
            LUT[i + 1][0] = '&';
        }
    }

    return;
}



//Main function to drive Huffman decoding of n encoded string
//Argument: LUT.txt file that contains a list of character/code pairs
int main(int argc, char *argv[]){

    if(argc!=3) {
		fprintf(stderr, "Argument Error: Expected ./decode LUT.txt encoded_input.txt\n");
		return(-1);
	}

    //open LUT text file to get character/code pairs for LUT data structure
    FILE* file = fopen(argv[1], "r");
    if(file == NULL){
      perror("Error opening LUT.txt file");
      return(-1);
    }

    //initialize the lookup tables with values from LUT.txt - stored as global LUTs
    build_lookup_tables(file);
    fclose(file);

    //open input (encoded) text file
    file = fopen(argv[2], "rb");
    if(file == NULL){
      perror("Error opening encoded_input.txt file");
      return(-1);
    }

    //decode the encoded file, output result to a new file
    //huffman_decode(file);

    //--- Huffman Decoding via ARM inline instruction ---

    //define variables for the ARM microprocessor
    char *output_str = malloc(2000 * sizeof(char));

    //get length of Huffman-encoded file - max file length = 2^16 characters
    fseek(file, 0, SEEK_END);
    uint16_t file_len = ftell(file);
    rewind(file);

    //allocate a buffer to store contents of entire file - extra byte for null terminator
    char* encoded_bit_str = malloc(sizeof(char) * (file_len + 1));
    size_t file_end_ptr = fread(encoded_bit_str, sizeof(char), file_len, file);
    if (ferror(file) != 0){
        fputs("Error reading the input file.", stderr);
        exit(1);
    }
    else{
        encoded_bit_str[file_end_ptr++] = '\0';
    }
    fclose(file);

    //perform Huffman decoding via inline ARM instruction
    //"r" allows gcc to keep variables in a general-purpose register, if available
    //"=r" specifies output operand for the assembly instruction
    __asm__("huffman %1, %2, %3, %4" : "=r" (output_str) : "r" (encoded_bit_str), "r" (main_LUT), "r" (extended_LUT));

    free(encoded_bit_str);
    free(output_str);
    return 0;
}
