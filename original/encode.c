#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Number of unique letters present in the French alphabet == max height of MinHeap

//#define ALPHABET_SIZE 80 i dont think this is used

#define CHECK_BIT(var,pos) ((var) & (1<<(pos))) //Baby function to check if a bit in a position is set
#define NUM_LETTERS 26 //Trying new alphabet of Len 26
#define MAX_CODE_LEN 40 //Reducing Max Len based on new alphabet

//-----GLOBALS-----
//alphabet LUT that holds the 2nd byte for all French UTF-8 letter in our language
int french_alphabet_LUT[NUM_LETTERS] = {
    128,
    130,
    135,
    136,
    137,
    138,
    139,
    142,
    143,
    148,
    153,
    155,
    156,
    160,
    162,
    167,
    168,
    169,
    170,
    171,
    174,
    175,
    180,
    185,
    187,
    188
};

//-----DATA STRUCTURES-----
//Huffman tree (MinHeap) node
typedef struct Node{

    unsigned char letter;     //the letter specified
    unsigned int freq;   //frequency of letter
    struct Node *left, *right;  //the Node's children - maximum 2
}Node;

//Min Heap: Collection of min-heap nodes stored in a tree-like structure.
//With Min Heap, the parent node always has a lesser value (freq, here) than the children. Root node has lowest value.
typedef struct MinHeap{
 
    unsigned int size;     //current size of min heap
    unsigned int capacity; //max capacity of heap
    struct Node** list;  //array (list) of pointers to Nodes
}MinHeap;

//Stack Node for iterative traversal of MinHeap structure (getting Huffman codes)
typedef struct StackNode{

  struct Node* node;
  struct StackNode* next;
}StackNode;

//Letter struct to return from get_huffman_codes
typedef struct Letter{

  unsigned char ch;
  unsigned int freq;
}Letter;

//Create a new blank Node for the MinHeap
struct Node* create_new_node(char letter, unsigned freq){

    //allocate memory for a Node
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
 
    node->left = node->right = NULL;
    node->letter = letter;
    node->freq = freq;

    return node;
}

//Create a new blank MinHeap structure
struct MinHeap* create_min_heap(unsigned int capacity){
 
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
 
    //allocate the max capacity amount of memory right away
    heap->list = (struct Node**)malloc(heap->capacity * sizeof(struct Node*));

    return heap;
}


//-----SUBROUTINES-----
/* MinHeapify to compare values for heap building - iterative algorithm for efficiency
*   - A min heap must have all possible subtrees as min heaps as well, by definition
*/
void iterative_min_heapify(struct MinHeap* heap, int index){

    int smallest;
    unsigned int left;
    unsigned int right;
    struct Node* temp;

    //event loop
    /*while(1){

        //set smallest variable & L/R subtree 
        smallest = index;
        left = 2 * index + 1;
        right = 2 * index + 2;

        //compare left subtree size & value to find Node w/ smallest freq value
        if(left < heap->size &&
            heap->list[left]->freq < heap->list[smallest]->freq){

            smallest = left;
        }

        //compare right subtree
        if(right < heap->size &&
            heap->list[right]->freq < heap->list[smallest]->freq){

            smallest = right;
        }

        //break case - the smallest value is at the original heap index passed, meaning subtree is heapified
        if(index == smallest){
            break;
        }

        //else - swap the Nodes at 'index' & 'smallest'
        temp = heap->list[smallest];
        heap->list[smallest] = heap->list[index];
        heap->list[index] = temp;
    }*/

    //try out recursive implementation of heapify function
    smallest = index;
    left = 2 * index + 1;
    right = 2 * index + 2;
 
    if (left < heap->size
        && heap->list[left]->freq
               < heap->list[smallest]->freq)
        smallest = left;
 
    if (right < heap->size
        && heap->list[right]->freq
               < heap->list[smallest]->freq)
        smallest = right;
 
    if (smallest != index) {
        //swap min heap node 
        temp = heap->list[smallest];
        heap->list[smallest] = heap->list[index];
        heap->list[index] = temp;
        iterative_min_heapify(heap, smallest);
    }

    return;
}


//Build MinHeap structure with frequency values loaded into new Node structs & inserted into MinHeap
struct MinHeap* build_min_heap(char letter[], unsigned int freq[], unsigned int size){
 
    //create a new minHeap struture
    struct MinHeap* heap = create_min_heap(size);
 
    //create a new Node for each unique character to receive a distinct code
    for (int i = 0; i < size; i++){
        heap->list[i] = create_new_node(letter[i], freq[i]);
    }
 
    heap->size = size;
    
    //build the min heap - compare all possible subtrees in order of largest size -> smallest size
    int n = size - 1; 
    for (int i = (n - 1) / 2; i >= 0; i--){
        iterative_min_heapify(heap, i);
    }

    return heap;
}


//Insert a Node into the MinHeap list
void insert_node(struct Node* node, struct MinHeap* heap){
    
    int i = heap->size;
    heap->size++;

    //check each child heap index with its parent heap index to find insertion location
    while(i && (node->freq < heap->list[(i - 1) / 2]->freq)){

        //swap list items to reorder: list index [i] & its parent at index [(i - 1) / 2]
        heap->list[i] = heap->list[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    //once correct location is found, insert node into correct list index
    heap->list[i] = node;
}


//Remove the minimum frequency-value Node from the MinHeap list
struct Node* remove_min_node(struct MinHeap* heap){

    //get minimum frequency Node
    struct Node* temp = heap->list[0];

    //replace list index 0 with the last element, then heapify again
    heap->list[0] = heap->list[heap->size - 1];
    heap->size--;
    iterative_min_heapify(heap, 0);
 
    return temp;
}


//Function that handles building the Huffman tree by storing a traversal
struct Node* build_huffman_tree(char letter[], int freq[], int size){

    struct Node *left_child, *right_child, *internal_node;

    //create the min heap data structure to organize the letters by highest frequency at root (ie. smallest code assigned)
    struct MinHeap* heap = build_min_heap(letter, freq, size);

    //iterate through each Node in heap & build the Huffman Tree
    while(heap->size > 1){
        //printf("%d\n", heap->size);

        //remove the 2 minimum frequency Nodes from the MinHeap list
        left_child = remove_min_node(heap);
        right_child = remove_min_node(heap);
        printf("Removed nodes: (%d with freq %d) & (%d with freq %d)\n", left_child->letter, left_child->freq, right_child->letter, right_child->freq);

        //create a new internal node on each iteration - placeholder character & sum of children's frequencies
        internal_node = create_new_node('0'-48, left_child->freq + right_child->freq);
 
        //set internal Node's children
        internal_node->left = left_child;
        internal_node->right = right_child;
 
        //insert internal node back into heap
        insert_node(internal_node, heap);
    }

    //final Node is the internal Node that contains the sum of all frequencies
    return remove_min_node(heap);
}


//Function to check if a given Node is a leaf node in the heap
int is_leaf_node(struct Node* node){
    return (!(node->left) && !(node->right));
}


//Function to push an item to the stack
void push(struct StackNode** top_ref, struct Node* node){

  //allocate node
  struct StackNode* new_stack_node = (struct StackNode*) malloc(sizeof(StackNode));
 
  if(new_stack_node == NULL){
    printf("Stack Overflow! \n");
    getchar();
    exit(0);
  }           
 
  //put in the new Node object which holds data (letter, freq, children)
  new_stack_node->node = node;
 
  //link the old list off the new Node
  new_stack_node->next = (*top_ref);  
 
  //move the head to point to the new Node
  (*top_ref) = new_stack_node;
}


//The function returns true if stack is empty, otherwise false
int is_empty(struct StackNode *top){
   return (top == NULL)? 1 : 0;
}  

 
//Function to pop an item from the stack
struct Node* pop(struct StackNode** top_ref){

  struct Node* result;
  struct StackNode* top;
 
  /*If StackNode is empty then error */
  if(is_empty(*top_ref)){
    printf("Stack Underflow \n");
    getchar();
    exit(0);
  }

  else{
    top = *top_ref;
    result = top->node;
    *top_ref = top->next;
    free(top);
    return result;
  }
}


// A utility function to print an array of size n
void print_array(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
 
    printf("\n");
}

// Taken from https://stackoverflow.com/questions/62361489/convert-int-array-to-string-using-c
// Not really sure how the sprintf statement works tbh
char* int_array_to_string(int array[],int n) {

    int i;
    char* output = (char*)malloc(128);
    char* point = output;
    for(i = 0; i != n;++i)
       point +=  sprintf(point,i+1!=n?"%d":"%d",array[i]);

    return output;
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
	unsigned char unsign_ch = ch - 128;
		
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
		unsign_ch = ch;
		if(unsign_ch < 127 && unsign_ch!=10){
			fprintf(stderr, "ASCII value %d passed. Don't pass any newlines, whitespace, a-zA-Z0-9 etc...", unsign_ch);
			exit(1);
		}
		//unsign_ch = ch - 128;
        for(int i = 0; i < NUM_LETTERS; i++){
            if(unsign_ch == french_alphabet_LUT[i]){
                arr[i] += 1;
                break;
            }
        }
		//arr[unsign_ch] += 1;
	}
	fclose(fp);
}

/*Function to receive a root Node of the Min Heap & output the list of Huffman Codes given to each character
*   - Traverses the Tree using an iterative InOrder traversal (without root node)
*   - code is built left to right, by left shifting the integer
*/
void get_huffman_codes(struct Node* root, int code[], int top, char *char_to_code[]){

    // Assign 0 to left edge and recur
    if (root->left) {
 
        code[top] = 0;
        get_huffman_codes(root->left, code, top + 1, char_to_code);
    }
 
    // Assign 1 to right edge and recur
    if (root->right) {
 
        code[top] = 1;
        get_huffman_codes(root->right, code, top + 1, char_to_code);
    }
 
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (is_leaf_node(root)) {
        //printf("%c: ", index);
	//print_array(code, top);
	
	    unsigned char index = root->letter;
  	    char_to_code[index]= int_array_to_string(code,top);
        printf("Letter: '%d' with code %s\n", index, char_to_code[index]);
    }

    return;
}

//Function to show the Nodes of a tree in InOrder traversal order
void Tree_inOrder(Node* n){

    if(n==0){
        return;
    }

    Tree_inOrder(n->left);
    if(is_leaf_node(n)){
	    printf("Tree InOrder Traversal - Letter: '%d' with frequency %d\n", n->letter, n->freq);
    }
    Tree_inOrder(n->right);

}

void generate_LUT(char* lut_file, char *char_to_code[]){

	FILE *lut_fp = fopen(lut_file, "wb");
	if(lut_fp == NULL) {
		fprintf(stderr, "File Error: Could not open %s\n", lut_file);
		exit(2);
	}
	
    //print first line of LUT, denoting min and max code size
    //fprintf(lut_fp, "%d, %d\n", i, char_to_code[i]);

    //print to LUT file all indices for 26 valid French characters in input2.txt (2nd byte acts as index)
    //checking is needed so we only add characters to the LUT if they appeared in the original input file
    short max_code_len;
    short min_code_len;
    short first_code_seen = 0;
    for(int i = 0; i < 200; i++){

        if(char_to_code[i] != NULL){

            if(!first_code_seen){
                max_code_len = strlen(char_to_code[i]);
                min_code_len = strlen(char_to_code[i]);

                //get length of first index to print to avoid overwriting when reprinting first line for input.txt.lut
                char index[5];
                //itoa(i, index, 10); AVOID itoa() non-standard
                snprintf(index, 10, "%d", i);
                int index_len = strlen(index);
                
                //print as many characters as index len + 1 (for trailing comma)
                for(int j = 0; j < index_len + 1; j++){
                    fprintf(lut_fp, "\n");
                }

                first_code_seen = 1;
            }

            else if(strlen(char_to_code[i]) > max_code_len){
                max_code_len = strlen(char_to_code[i]);
            }
            else if(strlen(char_to_code[i]) < min_code_len){
                min_code_len = strlen(char_to_code[i]);
            }

            fprintf(lut_fp, "%d,%s\n", i, char_to_code[i]);
        }
    }

    //finally, print the min/max lengths to the first line of file
    rewind(lut_fp);
    fprintf(lut_fp, "%d,%d\n", min_code_len, max_code_len);

        /*fprintf(lut_fp, "%d, %s\n", 128, char_to_code[128]);
        fprintf(lut_fp, "%d, %s\n", 130, char_to_code[130]); 
        fprintf(lut_fp, "%d, %s\n", 135, char_to_code[135]);
        fprintf(lut_fp, "%d, %s\n", 136, char_to_code[136]); 
        fprintf(lut_fp, "%d, %s\n", 137, char_to_code[137]); 
        fprintf(lut_fp, "%d, %s\n", 138, char_to_code[138]); 
        fprintf(lut_fp, "%d, %s\n", 139, char_to_code[139]); 
        fprintf(lut_fp, "%d, %s\n", 142, char_to_code[142]); 
        fprintf(lut_fp, "%d, %s\n", 143, char_to_code[143]); 
        fprintf(lut_fp, "%d, %s\n", 148, char_to_code[148]); 
        fprintf(lut_fp, "%d, %s\n", 153, char_to_code[153]); 
        fprintf(lut_fp, "%d, %s\n", 155, char_to_code[155]);
        fprintf(lut_fp, "%d, %s\n", 156, char_to_code[156]); 
        fprintf(lut_fp, "%d, %s\n", 160, char_to_code[160]);
        fprintf(lut_fp, "%d, %s\n", 162, char_to_code[162]); 
        fprintf(lut_fp, "%d, %s\n", 167, char_to_code[167]);
        fprintf(lut_fp, "%d, %s\n", 168, char_to_code[168]); 
        fprintf(lut_fp, "%d, %s\n", 169, char_to_code[169]); 
        fprintf(lut_fp, "%d, %s\n", 170, char_to_code[170]); 
        fprintf(lut_fp, "%d, %s\n", 171, char_to_code[171]); 
        fprintf(lut_fp, "%d, %s\n", 174, char_to_code[174]); 
        fprintf(lut_fp, "%d, %s\n", 175, char_to_code[175]);
        fprintf(lut_fp, "%d, %s\n", 180, char_to_code[180]);
        fprintf(lut_fp, "%d, %s\n", 185, char_to_code[185]); 
        fprintf(lut_fp, "%d, %s\n", 187, char_to_code[187]); 
        fprintf(lut_fp, "%d, %s\n", 188, char_to_code[188]);*/
}

//Function to Huffman encode file and output it to a binary file, given the code translations
void encode_file(char* in_file, char* out_file, char *char_to_code[]){

	FILE *in_fp = fopen(in_file, "r");
	if(in_fp == NULL) {
		fprintf(stderr, "File Error: %s does not exist\n", in_file);
		exit(2);
	}
	
	FILE *out_fp = fopen(out_file, "wb");
	if(out_fp == NULL) {
		fprintf(stderr, "File Error: Could not open %s\n", out_file);
		exit(2);
	}

	char ch;
	
	while((ch = fgetc(in_fp)) != EOF){
		if(CHECK_BIT(ch, 7)){ //checks if 8th bit is 1
			ch = fgetc(in_fp);
		}
		unsigned char unsign_ch = ch; //removed -128 here
        printf("%d\n", unsign_ch);
		char *code = char_to_code[unsign_ch];
		fputs(code, out_fp);
	}
	
	fclose(in_fp);
        
	
	//Convert each 8 bits to byte
	//This is not necesssary as we only want the bits represented as chars

/*	char binary_string[2000];
	binary_string[0]='\0';
	int i=0;
	int byte;
	char *ptr; //I don't know why i need this for strol
	char substring[8];
	substring[0] = '\0';

	for(i=0;i<strlen(binary_string);i+=8){
		strncpy(substring,&binary_string[i], 7);
		//printf("%s\n", binary_string);
		printf("%s\n", substring);
		byte = strtol(substring, &ptr, 2);
		//printf("%x ",byte);
		fputc(byte, out_fp);
	}
*/

	fclose(out_fp);

}
//Main function to drive the Huffman Tree building
//Takes input.txt and generates input.txt.huf and input.txt.lut
int main(int argc, char *argv[]) {

    if(argc!=2) {
        fprintf(stderr, "Argument Error: Expected ./encode input.txt\n");
	    return 1;
    }

    //Adding .huf and .lut extensions
    //****** -> I hate working with strings in C <- ******
    int f_len = strlen(argv[1]);
    char *in_file, out_file[f_len+4], lut_file[f_len+4];
    in_file = argv[1];
    strncpy(out_file, argv[1], f_len);
    strncpy(lut_file, argv[1], f_len);
    out_file[f_len] = '\0';
    lut_file[f_len] = '\0';
    strncat(out_file, ".huf", 4);
    strncat(lut_file, ".lut", 4);

    int freq_values[NUM_LETTERS] = {0}; //initialize array to 0's

    get_freq_values(in_file, freq_values);

    unsigned char letter[NUM_LETTERS] = {0}; //value is now the French character's 2nd byte value
    int i;

    //creating letters for the French alphabet specifically
    for(i=0;i<NUM_LETTERS;i++){
	    letter[i] = french_alphabet_LUT[i];
    }

    //view the frequencies
    /*for(i=0;i<NUM_LETTERS;i++){
	    printf("Letter: %d, Freq value: %d\n", letter[i], freq_values[i]);
    }*/

    //for the case that some characters do not ever appear, remove values from the arrays
    int found_freq_values[NUM_LETTERS];
    unsigned char found_letter[NUM_LETTERS];
    int size = 0;
    for(i=0;i<NUM_LETTERS;i++){
        if(freq_values[i] != 0){
            found_freq_values[size] = freq_values[i];
            found_letter[size] = letter[i];
            size++;
        }
    }
    found_freq_values[size] = 0;
    found_letter[size] = '\0';


    //create Huffman Tree containing only seen characters
    struct Node* tree_root = build_huffman_tree(found_letter, found_freq_values, size);
    Tree_inOrder(tree_root);

    int code[MAX_CODE_LEN];
    char *char_to_code[200] = { 0 };
    get_huffman_codes(tree_root, code, 0, char_to_code);

    //print temporary char arr
    for(i=0;i<200;i++){
        if(char_to_code[i] != NULL){
	        printf("Letter: %d, Code: %s\n", i, char_to_code[i]);
        }
        else{
            printf("Letter: %d, Code: NULL\n", i);
        }
    }

    //map the characters in char_to_code array to the 26-length French alphabet
    /*char* mapped_code[NUM_LETTERS] = { 0 };
    int j = 0;
    for(i = 0; i < 200; i++){
        if(char_to_code[i] != NULL){
            mapped_code[j] = char_to_code[i];
            j++;
        }
    }

    for(i=0;i<NUM_LETTERS;i++){
	    printf("MAPPED: Letter: %d, Code: %s\n", i, mapped_code[i]);
    }*/

    //TO FIX
    generate_LUT(lut_file, char_to_code);

    //TO FIX
    encode_file(in_file, out_file, char_to_code);

    return 0;
}
