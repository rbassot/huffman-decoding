#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Number of unique letters present in the French alphabet == max height of MinHeap
#define ALPHABET_SIZE 80


//-----DATA STRUCTURES-----
//Huffman tree (MinHeap) node
typedef struct Node{

    char letter;     //the letter specified
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
    heap->list[0] = heap->list[(heap->size) - 1];
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
        printf("%d\n", heap->size);

        //remove the 2 minimum frequency Nodes from the MinHeap list
        left_child = remove_min_node(heap);
        right_child = remove_min_node(heap);

        //create a new internal node on each iteration - placeholder character & sum of children's frequencies
        internal_node = create_new_node('!', left_child->freq + right_child->freq);
 
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
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
 
    printf("\n");
}


/*Function to receive a root Node of the Min Heap & output the list of Huffman Codes given to each character
*   - Traverses the Tree using an iterative InOrder traversal (without root node)
*   - code is built left to right, by left shifting the integer
*/
void get_huffman_codes(struct Node* root, int codes[], int top){

    // Assign 0 to left edge and recur
    if (root->left) {
 
        codes[top] = 0;
        get_huffman_codes(root->left, codes, top + 1);
    }
 
    // Assign 1 to right edge and recur
    if (root->right) {
 
        codes[top] = 1;
        get_huffman_codes(root->right, codes, top + 1);
    }
 
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (is_leaf_node(root)) {
 
        printf("%c: ", root->letter);
        printArr(codes, top);
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
        printf("Letter: '%c' with frequency %d\n", n->letter, n->freq);
    }
    Tree_inOrder(n->right);

} 


//Main function to drive the Huffman Tree building
int main(){
 
    //a is least probable - y most probable
    char letter[] = {'a', 'e', 'i', 'o', 'u', 'y'};
    int freq[] = {6, 10, 17, 25, 36, 55};
    int size = sizeof(letter) / sizeof(letter[0]);

    //TODO: call Tree building here
    struct Node* tree_root = build_huffman_tree(letter, freq, size);
    Tree_inOrder(tree_root);

    int codes[6]; //ALPHABET_SIZE
    get_huffman_codes(tree_root, codes, 0);

    //codes currently get printed to console
 
    return 0;
}

