#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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


//main
//takes input file and output file as args
int main(int argc, char *argv[]) {
	
	if(argc!=3) {
		fprintf(stderr, "Argument Error: Expected ./huffman input.txt output.bin\n");
		return 1;
	}
	else {
		printf("Args supplied: %s, %s\n", argv[1], argv[2]);
	}

	FILE *input = fopen(argv[1], "r");
	//FILE *output = fopen(argv[2], "w");
	//currently printing output to terminal for debugging
	//remove comments for final submission
	if(input == NULL) {
		fprintf(stderr, "File Error: %s does not exist\n", argv[1]);
		return 2;
	}

	char ch;
	while((ch = fgetc(input)) != EOF) {
		printf("%c", ch);
	}

	fclose(input);



	return 0;
}
