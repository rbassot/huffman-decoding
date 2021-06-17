#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>

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
int main(int argc, char *argv[]) {

   //set the locale to French for printing accented characters
   setlocale(LC_CTYPE,"UTF-8");

   //check that a file was passed
   if(argc < 2) {
      printf("There was no file passed as argument to the program.\n");
      exit(1);
   }

   //open the file that was passed as argument
   FILE* fp = fopen(argv[1], "rb, ccs=UTF-8");

   //check valid file
   if(fp == NULL){
      printf("There was an error and the file could not be opened for reading.\n");
      exit(1);
   }
   
   //build the min heap structure from the contents of the file
   initialize_min_heap(fp);
 
   return 0;
}

/*
Function to handle building the Min Heap data structure.
*/
void initialize_min_heap(FILE* fp){

   struct Node* root;
   char* data;

   //handle the file, then read the entire contents of the file & store to buffer
   data = parse_input_file(fp);

   //root = build_min_heap();
}

/*
Function to handle parsing of the input file at the command line.
*/
char* parse_input_file(FILE* fp){

   //count the number of bytes that need to be allocated in memory
   int bytes = 0;
   wchar_t c;
   while((c = fgetwc(fp)) != WEOF){
      wprintf(L"%lc\n", c);
      bytes++;
   }
   printf("File size:                %d bytes\n", bytes);

   //allocate memory for the file string & read file
   char* file_string = malloc(bytes + 1); //add space for null terminator
   rewind(fp);
   fread(file_string, 1, bytes, fp);
   fclose(fp);
   
   file_string[bytes] = 0;
   //printf("%s\n", file_string);

   return file_string;
}