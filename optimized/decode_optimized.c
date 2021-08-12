#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <locale.h>


/*
* REQUIRED SETUP STEPS:
*   1 - Set HWIDTH to a reasonable value to keep the main LUT to a reasonable size
*   2 - Set MAXWIDTH to the maximum length Huffman code existing in LUT.txt. This must be known beforehand
*   3 - In build_lookup_table() & GLOBALS, change main LUT & extended LUT types to handle respective code values that
*           are seen (i.e. uint8_t & uint_16t are currently used). All instances of these type declarations must be changed.
*/

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

    unsigned int i; //to avoid needing C99 compile flag
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

    /* LOOP UNROLLING applied 
     * Optimized i = 0      */
    
    for(i^=i ; i < LUT_SIZE; i += 2){

        if(LUT[i][1] == 0){
            
            LUT[i][0] = '&';
        }
        if(LUT[i + 1][1] == 0){
            
            LUT[i + 1][0] = '&';
        }
    }

    return;
}


/*Huffman decoding of the provided input file. This function writes to a new file called decoded_output.txt, and will rewrite
* existing files of that name.
*/
void huffman_decode(FILE* input_fp){

    //set locale to FR for printing French UTF-8 to file
    setlocale(LC_CTYPE, "");

    //create file pointer to output file
    FILE* output_fp;
    output_fp = fopen("./decoded_output.txt", "w");

    //get length of input file - max file length = 2^16 characters
    fseek(input_fp, 0, SEEK_END);
    uint16_t file_len = ftell(input_fp);
    rewind(input_fp);

    //allocate a buffer to store contents of entire file - extra byte for null terminator
    char* str_buffer = malloc(sizeof(char) * (file_len + 1));
    size_t file_end_ptr = fread(str_buffer, sizeof(char), file_len, input_fp);
    if (ferror(input_fp) != 0){
        fputs("Error reading the input file.", stderr);
        exit(1);
    }
    else{
        str_buffer[file_end_ptr++] = '\0';
    }


    /* Optimize Local/Register variables where possible */
    //initialize required variables for decoding
    char code_str[MAXWIDTH + 1];
    unsigned short code; //max value here is: 2^MAXWIDTH, therefore short type is wide enough for our alphabet
    register unsigned int i; //heavily used array index - keeping loop counters as int type to optimize 32-bit addition
    unsigned int j;
    register unsigned int decoded_shift = 0;  //heavily incremented shift marker
    register uint16_t decoded_letter; //heavily assigned decoded letter variable
    char* end_ptr;

    //decoding loop - ends when all bits of the full encoded string were 'shifted' off
    while(decoded_shift < file_len){

        /* LOOP UNROLLING applied - currently allows EVEN valued HWIDTH */
        //get code of length HWIDTH from the buffer (req'd for indexing main LUT)
        for(i = 0; i < HWIDTH; i += 2){
            code_str[i] = str_buffer[i + decoded_shift];
            code_str[i + 1] = str_buffer[i + 1 + decoded_shift];
        }
        code_str[i] = '\0';

        //convert code string to numeric index
        code = (short)strtol(code_str, &end_ptr, 2);

        //ensure that the entire code was converted
        if(*end_ptr != '\0'){
            fputs("Code passed was not null-terminated. Exiting program", stderr);
            exit(1);
        }

        //perform lookup table access, where the Huffman code is the table index
        decoded_letter = LUT[code][0];
        
        //Extended LUT case: check if extended LUT needs to be accessed - if so, grab the next required encoded bits
        if((decoded_letter ^ 38) == 0){ //compares to '&'

            /* LOOP UNROLLING applied - currently allows EVEN valued MAXWIDTH and HWIDTH */
            //get more bits, which add up to MAXLENGTH encoded bits (req'd for indexing extended LUT)
            for(j = 0; j < (MAXWIDTH - HWIDTH); j += 2){
                code_str[i + j] = str_buffer[i + decoded_shift + j];
                code_str[i + j + 1] = str_buffer[i + decoded_shift + j + 1];
            }
            code_str[i + j] = '\0';
            code = (short)strtol(code_str, &end_ptr, 2);

            if(*end_ptr != '\0'){
                fputs("Code passed was not null-terminated. Exiting program", stderr);
                exit(1);
            }

            //perform lookup on extended LUT
            decoded_letter = extended_LUT[code][0];
            decoded_shift += extended_LUT[code][1];
        }

        //Regular case: keep the original decoded letter & apply original shift value
        else{
            decoded_shift += LUT[code][1];
        }

        //tack on the first byte of the UTF-8 character, then print to file 
	    fprintf(output_fp, "%c%c", 195, decoded_letter);
    }

    fclose(output_fp);

    //free allocated memory
    free(str_buffer);
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
    huffman_decode(file);
    fclose(file);

    return 0;
}
