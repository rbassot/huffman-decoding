# SENG440 - French Language Huffman Decoding Progress Report
Team ID: 23
Daniel Kinahan, Raphael Bassot
V00887329, V00896541

## Specifications
This project involves leveraging an ARM-based processor to create an efficient, variable-length Huffman encoding/decoding algorithm in C programming language, with potential software optimizations done in ARM assembly. Our encoding algorithm is optimized based on the transmission rate - the frequency it will appear - of any given symbol. More frequently occurring symbols will dynamically receive a much shorter Huffman code than their less frequent counterparts. This uses a minimum heap data structure (implemented in C as a priority-queue), which maximizes the amount of binary codes produced per bit (see work completed section below).


## Work Completed
- Huffman minimum heap/priority queue building in C, to order the symbols by frequency (least frequent occurring at the heap's root)

- Huffman tree building and Huffman code assignments in C, using an InOrder traversal of the Huffman tree where all symbols are pushed to leaf nodes of the tree & every unique path to a node represents that symbol's unique code assignment with unused binary prefix.

- Calculation of our optimized transmission rate for the bits needed to represent Huffman codes, compared to the proposed encoding in the SENG440 Huffman Slides:

Our Optimized Huffman Encoding; equal in optimization to the SENG440 Huffman Slides:
(Example code table)
| Symbol      | Bit Combination (Code)   | Code Length | 
| ----------- | ------------------------ | ----------- |
| A           | 0                        | 1           |
| B           | 10                       | 2           |
| C           | 110                      | 3           |
| D           | 1110                     | 4           |
| E           | 1111                     | 4           |
| ----------- | ------------------------ | ----------- |

Average Codeword Transmission Rate = 2 bits/cycle
(0.4×1 + 0.3×2 + 0.15×3 + 0.1×4 + 0.05×4 ≈ 2)


## Solution Approach


## Processor
-2kb data memory + heap
-hardware LUT's
-16 bit
-https://global.epson.com/products_and_drivers/semicon/products/micro_controller/16bit/
## Questions

