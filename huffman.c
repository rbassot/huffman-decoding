#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define NUM_LETTERS 191
//max height == # of unique letters present in the French alphabet
#define MAX_TREE_HT 80

//-----DATA STRUCTURES-----
//Huffman tree node
struct Node{

    char letter;     //the letter specified
    unsigned freq;   //freq of letter
    struct MinHeapNode *left, *right;  //the node's children
};

//Min Heap: Collection of min-heap nodes stored in a tree-like structure.
//With Min Heap, the parent node always has a lesser value (freq, here) than the children. Root node has lowest value.
struct MinHeap{
 
    unsigned size;     //current size of min heap
    unsigned capacity; //max capacity of heap
    struct Node** list;  //array of pointers to nodes
};


//-----PROTOTYPES-----
void initialize_min_heap(FILE* fp);
char* parse_input_file(FILE* fp);

void print_array(int* array){

	int i;
	for(i=0;i<NUM_LETTERS;i++){
		printf("%d: %d\n", i, array[i]);
	}
}

//Function for getting frequency of each letter
//Takes file input and buffer
//Return frequency array where each index corresponds to the char->int conversion.
void get_freq_values(char* filename, int *arr){
	
	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
		fprintf(stderr, "File Error: %s does not exist\n", filename);
		exit(2);
	}
	
	char ch;
	
	//if byte begins with "1" (unicode double bytes begin with "110....")
	//	it is a double byte char
	//	use the second byte as address
	//else (it would begin with 0)
	//	ascii value is the address
	//Assumptions: 
	//	no triple or quad byte chars are given
	//	valid unicode is provided
	while((ch = fgetc(fp)) != EOF) {
			
		if(CHECK_BIT(ch, 7)){ //checks if 8th bit is 1
			ch = fgetc(fp);
		}
		unsigned char unsign_ch = ch;
		//printf("%d ", unsign_ch);
		arr[ch] += 1;
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {

	if(argc!=3) {
		fprintf(stderr, "Argument Error: Expected ./huffman input.txt output.bin\n");
		return 1;
	}
	
	int freq_values[NUM_LETTERS] = {0}; //initialize array to 0's
	
	get_freq_values(argv[1], freq_values);

	//print_array(freq_values);
	
	//code to write to file
	//FILE *output = fopen(argv[2], "w");
	
	return 0;
}


