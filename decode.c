#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/*
* REQUIRED SETUP STEPS:
*   1 - Set HWIDTH to a reasonable value to keep the main LUT to a reasonable size
*   2 - Set MAXWIDTH to the maximum length Huffman code existing in LUT.txt. This must be known beforehand
*   3 - In build_lookup_table() & GLOBALS, change main LUT & extended LUT types to handle respective code values that
*           are seen (i.e. uint8_t & uint_16t are currently used). All instances of these type declarations must be changed.
*/

#define FRENCH_ALPHABET_SIZE 35
#define HWIDTH 3 //This dictates the largest index size of the first LUT - any Huffman Codes longer will be handled
                      //through a pointer to the extended LUT
                      //**As long as this power is less than 8, a LUT of type uint8_t can be used!**

#define MAXWIDTH 4 //This dictates the largest index size of the extended LUT - The power is equivalent to
                        //the length of the longest existing Huffman code

#define LUT_SIZE 1 << HWIDTH
#define EXT_LUT_SIZE 1 << MAXWIDTH


/*-----GLOBALS-----*/
//Main LUT
uint8_t LUT[LUT_SIZE][2] = { 0 };

//Extended LUT
uint16_t extended_LUT[EXT_LUT_SIZE][2] = { 0 };


void build_lookup_tables(FILE* file){

    char line[256];
    int min_code_len;
    const int max_code_len;
    char huff_letter;
    char huff_code[MAXWIDTH + 1];

    char code_len;
    char* end_ptr;

    //get first line of file, which dictates the min & max code length
    fgets(line, sizeof(line), file);
    sscanf(line, "%d,%d", &min_code_len, 
            &max_code_len);

    printf("%d, %d\n", min_code_len, max_code_len);

    //read rest of file line by line
    while(fgets(line, sizeof(line), file)){
        //get values
        sscanf(line, "%c,%s", &huff_letter, &huff_code);

        //get length of current Huffman code - for storage in the LUT
        code_len = strlen(huff_code);

        //Case 1 - slot character & code length pair into main LUT
        if(code_len <= HWIDTH){
            //convert string code (binary representation) to number
            uint8_t num_huff_code;
            num_huff_code = (uint8_t)strtol(huff_code, &end_ptr, 2);
            //printf("Numeric: %d\n", num_huff_code);

            //slot character & code length into the table
            LUT[num_huff_code][0] = huff_letter;
            LUT[num_huff_code][1] = code_len;
        }

        //Case 2 - slot character & code length pair into extended LUT
        else{
            //convert string code (binary representation) to number
            uint16_t num_huff_code;
            num_huff_code = (uint16_t)strtol(huff_code, &end_ptr, 2);
            //printf("Numeric: %d\n", num_huff_code);

            //slot character & code length into the extended lookup table
            extended_LUT[num_huff_code][0] = huff_letter;
            extended_LUT[num_huff_code][1] = code_len;
        }
    }

    //iterate through both LUTs to fill in empty cells - this is necessary to obtain O(1) access with any code encountered
    //main LUT filling
    int prev_letter;
    int prev_code_len;
    int prev_code;
    for(int i = 0; i < LUT_SIZE; i++){

        if(LUT[i][1] != 0){
            //check prefix to determine if this index is also valid for the given letter
            prev_letter = LUT[i][0];
            prev_code_len = LUT[i][1];
            prev_code = i;
            continue;
        }

        else{
            /*check prefix to determine if this index is also valid for the given letter:
            *   bit shift the current code to compare its prefix with the previous index's prefix
            */
            if(((i >> (HWIDTH - prev_code_len)) ^ (prev_code >> (HWIDTH - prev_code_len))) == 0){
                LUT[i][0] = prev_letter;
                LUT[i][1] = prev_code_len;
            }

            //any remaining empty cells in main LUT require a marker to point to the extended LUT
            else{
                LUT[i][0] = '&';
                LUT[i][1] = HWIDTH;
            }
        }
    }


    //extended LUT filling
    prev_letter = prev_code_len = prev_code = 0;
    for(int i = 0; i < EXT_LUT_SIZE; i++){

        if(extended_LUT[i][1] != 0){
            //get the previous values for duplication across following empty table cells
            prev_letter = extended_LUT[i][0];
            prev_code_len = extended_LUT[i][1];
            prev_code = i;
            continue;
        }

        else{
            /*check prefix to determine if this index is also valid for the given letter:
            *   bit shift the current code to compare its prefix with the **previous index's prefix**
            */
            if(((i >> (MAXWIDTH - prev_code_len)) ^ (prev_code >> (MAXWIDTH - prev_code_len))) == 0){
                extended_LUT[i][0] = prev_letter;
                extended_LUT[i][1] = prev_code_len;
            }
        }
    }

    //try printing main LUT character/code length pairs
    printf("---MAIN LUT---\n");
    printf("INDEX | LETTER, CODE_LEN\n");
    for(int i=0; i < LUT_SIZE; i++){
        printf("%d | %c, %d\n", i, LUT[i][0], LUT[i][1]);
    }
    printf("\n");
    printf("---EXTENDED LUT---\n");
    printf("INDEX | LETTER, CODE_LEN\n");
    for(int i=0; i < EXT_LUT_SIZE; i++){
        printf("%d | %c, %d\n", i, extended_LUT[i][0], extended_LUT[i][1]);
    }

    return;
}


/*Huffman decoding of the provided input file. This function writes to a new file called decoded_output.txt, and will rewrite
* existing files of that name.
*/
void huffman_decode(FILE* input_fp){

    //create file pointer to output file
    FILE* output_fp;
    output_fp = fopen("decoded_output.txt", "w");

    //get length of input file
    long file_len;
    fseek(input_fp, 0, SEEK_END);
    file_len = ftell(input_fp);
    printf("File length in bytes/chars: %i\n", file_len);
    rewind(input_fp);

    //allocate a buffer to store contents of entire file - extra byte for null terminator
    char* str_buffer = malloc(sizeof(char) * (file_len + 1));
    size_t end_point = fread(str_buffer, sizeof(char), file_len, input_fp);
    if (ferror(input_fp) != 0){
        fputs("Error reading file", stderr);
        exit(1);
    }
    else{
        str_buffer[end_point++] = '\0';
    }
    printf("%s\n", str_buffer);

    char code_str[MAXWIDTH + 1];
    int code;
    int i;
    int decoded_shift = 0;
    char* end_ptr;
    char decoded_letter;

    //decoding loop
    while(true){

        //get code of length HWIDTH from the buffer
        for(i = 0; i < HWIDTH; i++){
            code_str[i] = str_buffer[i + decoded_shift];
        }
        code_str[i] = '\0';
        printf("Current code str: %s\n", code_str);

        //convert code string to numeric index
        code = strtol(code_str, &end_ptr, 2);

        //ensure that the entire code was converted
        if(*end_ptr != '\0'){
            fputs("Code passed was not null-terminated. Exiting program", stderr);
            exit(1);
        }

        printf("Current numeric code: %d\n", code);

        //perform lookup table access, where the Huffman code is the table index
        decoded_letter = LUT[code][0];
        decoded_shift += LUT[code][1];

        //TODO: create function to handle writing French UTF-8 characters to the output file

        exit(0);
    }

    fclose(output_fp);
}


//Main function to drive Huffman decoding of n encoded string
//Argument: LUT.txt file that contains a list of character/code pairs
int main(int argc, char *argv[]){

    if(argc!=3) {
		fprintf(stderr, "Argument Error: Expected ./decode LUT.txt encoded_input.txt\n");
		return 1;
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

    //decode the encoded input string from input file
    file = fopen(argv[2], "rb");
    if(file == NULL){
      perror("Error opening encoded_input.txt file");
      return(-1);
    }

    //decode the encoded file
    huffman_decode(file);
    fclose(file);

    return 0;
}