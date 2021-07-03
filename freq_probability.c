#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//max height == # of unique letters present in the French alphabet
#define MAX_TREE_HT 80


//-----DATA STRUCTURES-----
//Huffman tree (MinHeap) node
struct Node{

    char letter;     //the letter specified
    unsigned int freq;   //frequency of letter
    struct Node *left, *right;  //the Node's children - maximum 2
};

//Min Heap: Collection of min-heap nodes stored in a tree-like structure.
//With Min Heap, the parent node always has a lesser value (freq, here) than the children. Root node has lowest value.
struct MinHeap{
 
    unsigned int size;     //current size of min heap
    unsigned int capacity; //max capacity of heap
    struct Node** list;  //array (list) of pointers to Nodes
};

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
    while(1){

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


//Insert a Node into the MinHeap
void insert_node(){
    
}

//Function that handles building the Huffman tree by storing a traversal
struct Node* buildHuffmanTree(char letter[], int freq[], int size){

    struct Node *left_child, *right_child, *internal_node;

    //create the min heap data structure to organize the letters by lowest frequency at root
    struct MinHeap* heap = build_min_heap(letter, freq, size);

    //iterate through heap & build the Huffman Tree
    while(heap->size > 1){

    }
}


//Main function to drive the Huffman Tree building
int main(){
 
    //a is least probable - f most probable
    char letter[] = { 'a', 'e', 'i', 'o', 'u', 'y' };
    int freq[] = { 6, 10, 17, 25, 36, 55 };
    int size = sizeof(letter) / sizeof(letter[0]);
 
    //TODO: call Tree building here
    struct MinHeapNode* tree_root = buildHuffmanTree(letter, freq, size);
 
    return 0;
}

