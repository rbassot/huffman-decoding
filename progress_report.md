# SENG440 - French Language Huffman Decoding Progress Report
Team ID: 23
Daniel Kinahan, Raphael Bassot
V00887329, V00896541

## Specifications
This project involves leveraging an ARM-based processor to create an efficient, variable-length Huffman encoding/decoding algorithm in C programming language, with potential software optimizations done in ARM assembly. Our encoding algorithm is optimized based on the transmission rate - the frequency it will appear - of any given symbol. More frequently occurring symbols will dynamically receive a much shorter Huffman code than their less frequent counterparts. This uses a minimum heap data structure (implemented in C as a priority-queue), which maximizes the amount of binary codes produced per bit (see work completed section below).


## Work Completed
- Parsing file for frequency and storing in an array

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
This project uses the Basic Latin and Latin-1 Supplement set of Unicode values. The standard ASCII set of characters are stored using a single Unicode byte and all acccented French characters consist of two-byte Unicode values. The array generated uses the ASCII values for non accented characters or the second bytes value for French as the indeces and the value as the frequency (e.g 'é' has the value hex-C3A9, which places it at the index hex-A9 or decimal-169). This gives us O(n) efficicency for generating the array and O(1) for access time.
#### Example
The string 'AAAB' would give us the array with value 3 at index 65 and 1 at index 66.
| **Index** | 0...64 | 65 | 66 | 67..191 |
|:-----:|:------:|:--:|:--:|:-------:|
| **Value** |    0   |  3 |  1 |    0    |


## Processor
To store a Look Up Table with 191 entries in system memory requires roughly 4kb. This will be the lower bound for the processors cache memory as storage in RAM could introduce delay. One with hardware LUT's would be ideal to reduce access time. The processor will need a 16-bit word length as the decoding requires printing two-byte Unicode values.

We have chosen the Freescale Semiconductor Kinetis MKL36Z64VLH4 as it exceeds the system cache memory requirement with 8kb and has a 32-bit word length. This  ARM Cortex-M0+ core MCU is priced at $3.10769 per unit on digikey. It operates at 48Mhz, with low power consumption. This processor also includes many additional features that are extraneous to this project.


-https://global.epson.com/products_and_drivers/semicon/products/micro_controller/16bit/
## Questions

