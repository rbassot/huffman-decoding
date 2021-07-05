# SENG440 - French Language Huffman Decoding Progress Report
Team ID: 23
Daniel Kinahan, Raphael Bassot
V00887329, V00896541


## Project Specifications
This project involves leveraging an ARM-based processor to create an efficient, variable-length Huffman encoding/decoding algorithm in C programming language, with potential software optimizations done in both C and ARM assembly. Our encoding algorithm is optimized based on the transmission rate - the frequency it will appear - of any given symbol. More frequently occurring symbols will dynamically receive a much shorter Huffman code than their less frequent counterparts. This encoding uses a minimum heap data structure (implemented in C as a priority-queue), which maximizes the amount of binary codes produced per bit (see work completed section below).

Our intended microprocessor architecture will be specialized to handle a large amount of LUT operations - such as accessing an index - which will be required to maximize efficiency of our decoding algorithm.


## Work Completed
1. Huffman minimum heap/priority queue building in C, to order the symbols by frequency (least frequent occurring at the heap's root)

2. Huffman tree building and Huffman code assignments in C, using an InOrder traversal of the Huffman tree where all symbols are pushed to leaf nodes of the tree & every unique path to a node represents that symbol's unique code assignment with unused binary prefix.


3. Calculation of our optimized transmission rate for the bits needed to represent Huffman codes, compared to the proposed encoding in the SENG440 Huffman Slides. This is done dynamically in our Huffman encoding, and will present results during decoding.

Our Optimized Huffman Encoding; equal in optimization to the provided SENG440 Huffman Slides:

(Example symbol probabilities, codes table, transmission calculations)
Given probabilities: A = 40%, B = 30%, C = 15%, D = 10%, E = 5%:

| Symbol      | Bit Combination (Code)   | Code Length | 
| ----------- | ------------------------ | ----------- |
| A           | 0                        | 1           |
| B           | 10                       | 2           |
| C           | 110                      | 3           |
| D           | 1110                     | 4           |
| E           | 1111                     | 4           |


### Without Our Huffman Coding
#### Average Amount of Information per Symbol Without Huffman Coding:
A = 000 -> Information of symbol A = -log2(0.4) = ~1.32 bits of info
B = 001 -> Information of symbol B = -log2(0.3) = ~1.74 bits of info
C = 010 -> Information of symbol C = -log2(0.15) = ~2.74 bits of info
D = 011 -> Information of symbol D = -log2(0.1) = ~3.32 bits of info
E = 100 -> Information of symbol E = -log2(0.05) = ~4.32 bits of info

We know: Average number of bits per symbol = sum[(bits needed per symbol i) * (probability of symbol i occurring)]
    = (1.32×0.4 + 1.74×0.3 + 2.74×0.15 + 3.32×0.1 + 4.32×0.05)
    = ~2.01 bits of information per symbol (N)

This means we know we are wasting bit space with this redundant representation - almost a whole bit of information is wasted, becasue each symbol requires 3 bits!


#### Average Transmission Rate Without Huffman Coding:
Avg. Transmission rate = (3 bits needed for each symbol) * (probability of any symbol)
    = 3 bits/cycle


### Using Our Huffman Coding
#### Average Huffman Codeword Transmission Rate
Avg. Transmission rate = sum[(bits needed per symbol i) * (probability of symbol i occurring)]
    = (0.4×1 + 0.3×2 + 0.15×3 + 0.1×4 + 0.05×4) = ~2 bits/cycle 

Therefore, with this basic example, using our Huffman encoding will generate a 33% lower transmission rate, and will maximize the amount of information per bit used in every symbol.


## Solution Approach
#### Huffman Encoding
This project uses the Basic Latin and Latin-1 Supplement set of Unicode values. The standard ASCII set of characters are stored using a single Unicode byte and all acccented French characters consist of two-byte Unicode values. The array generated uses the ASCII values for non accented characters or the second bytes value for French as the indeces and the value as the frequency (e.g 'é' has the value hex-C3A9, which places it at the index hex-A9 or decimal-169). This gives us O(n) efficicency for generating the array and O(1) for access time.
#### Example
The string 'AAAB' would give us the array with value 3 at index 65 and 1 at index 66.
| **Index** | 0...64 | 65 | 66 | 67..191 |
|:-----:|:------:|:--:|:--:|:-------:|
| **Value** |    0   |  3 |  1 |    0    |
Our solution to the Huffman encoding problem is to store all possible characters we may see (regular 128 ASCII + ~60 UTF-8 French/Latin characters) as one-byte integers (char types). Essentially, all regular ASCII will keep their values from 0-127, taking up 2^7 bits worth of alphabet symbols. Then, each UTF-8 character we amy also see when parsing French text will be stored sequentially between values of 128-191. As UTF-8 are actually 2-byte characters, we will only store a 1-byte reference to each character in our table of symbol occurrence when parsing a text. This restricts the indices to lookup/access when parsing a French input file, and takes much less space to store in memory.

Provided we have the frequency of occurrence of each possible French alphabet character, we will input this array of symbol+frequency pairs into a minimum heap/tree-traversal algorithm which dynamically builds an optimized table of Huffman codes, with minimal-length codes for the most frequently-occurring characters in that input file. This minimum heap data structure is implemented as a priority queue, which is kept in order iteratively as Nodes (symbol+frequency pairs) are added to the queue. This min-heap is then iterated through to create a Huffman tree structure with all Nodes at the tree's leaf node positions, and internal nodes filling the remaining tree positions. Doing this allows every unique symbol+frequency pair to have a distinct path from the root node to the leaf. By appending a '0' to the code on every move to the left child, and appending a '1' on moves to the right child, then a binary code is created upon traversal of a path, and this code is assigned to the Node as a Huffman code for that specific alphabet symbol. Each Huffman code is not only unique, but prefix-free, which means no code can be ambiguously interpreted at any point in a decoding execution. Currently, this tree traversal is an InOrder traversal that is performed recursively for ease of implementation - we intend on using iteration in the final project for efficiency benefits.


#### Huffman Decoding
For the Huffman decoding problem, we intend on leveraging a 16-bit microprocessor with ~4KB of local memory that is optimized to perform LookUp Table (LUT) operations, such as accessing Huffman codes by character index. Once a Huffman codes table is generated (or provided),the intended steps for decoding will be:

    - Determining the next symbol on-the-fly by reading a next input character (encoded digit) and accessing pairs in the LUT - this is possible as codes are all prefix-free, and all codes of only the currently examined length can be looked up to save resources

    - The decoded alphabet symbol will be appended to a file - this happens in the exact order that the symbols are decoded

    - Once a code match is determined, the input string is shifted over by the code's amount of bits - this represents discarding the portion of decoded symbol

The above 3 steps will be repeated until the entire Huffman-encoded input string is decoded, and the file will be provided as output.


## Processor
To store a Look Up Table with 191 entries in system memory requires roughly 4kb. This will be the lower bound for the processors cache memory as storage in RAM could introduce delay. One with hardware LUT's would be ideal to reduce access time. The processor will need a 16-bit word length as the decoding requires printing two-byte Unicode values.

We have chosen the Freescale Semiconductor Kinetis MKL36Z64VLH4 as it exceeds the system cache memory requirement with 8kb and has a 32-bit word length. This  ARM Cortex-M0+ core MCU is priced at $3.10769 per unit on digikey. It operates at 48Mhz, with low power consumption. This processor also includes many additional features that are extraneous to this project.


## Questions Towards our Project's Completion
- In a pure software solution for decoding, how would we implement caching, and what are the benefits towards Huffman Decoding? We are thinking of using an architecture that will maximize our effectiveness (as mentioned above), which would ideally include native LookUp Table support - how is caching different or better in this scenario?

- What portions of our above Huffman decoding algorithm use the CPU resources the least? To combat this, how do we maximize use of the CPU while the Huffman algorithm is busy decoding the input string? (ie. what should the CPU be doing meanwhile?)


