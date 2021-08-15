# Huffman Encoding
SENG440 project by Daniel Kinahan (V00887329) and Raphael Bassot (V00896541), of the University of Victoria

SENG440 Team ID: 23

Date: August 14, 2021

## Details
This Huffman Encoding project utilizes:

- The French extended alphabet of symbols for input alphabet. For the project test bench, a 300+ French UTF-8 character file, `input.txt`, was created using 15 distinct French characters. This file is encoded (by encode.c program), which generates the two files for input to the decoder: `input.txt.lut`, and `input.txt/huf`, both found in the `/original` folder of this repository.

- The probability distribution of the French alphabet letters in the input.txt file follows a normal distribution, to provide a varying frequency of occurrence for each character, allowing the Huffman encoding algorithm to generate optimal Huffman codes for each unique symbol.

- The optimized decoder, in `optimized/decode_optimized.c`, was cross-compiled into an ARM executable for execution on the UVic SENG440 arm device - an ARM920T microcontroller.

- For full theoretical background of the project, implementation of Huffman coding, software optimizations performed, and metrics collected, see the SENG440 Final Report PDF document submitted alongside this project.


## decode_optimized.c
The optimized Huffman decoder in `optimized/decode_optimized.c` is the main file for this project, optimized for use on the ARM920T microcontroller. The program performs two main operations: dynamically building the two lookup tables - main and extended - from the `.lut` file passed as input, and decoding the encoded file found in `.huf`. Decoding is performed in the `huffman_decode` function in an iterative process, symbol by symbol. Huffman codes are shifted out of the `.huf` file sequentially, where each code is resolved into its respective character by looking up the index of the code in the lookup table (LUT). The main lookup table comprises of the symbols matching ~90% of the codes seen during decoding; when a code does not exist in the main LUT, that code is used to lookup its character in the extended LUT, which contains the larger Huffman codes and therefore less frequently-occurring symbols. The resolved French symbols are then written individually to an output file, called `decoded_output.txt`. Once execution of the decoder is complete, the output file is fully decoded and matches the contents of the initial input file that was encoded.


## Executing the Huffman Decoder on the ARM Device
1. Copy the three files - `optimized/decode_optimized.c`, `original/input.txt.lut`, & `original/input.txt.huf` - from a local machine onto the UVic SENG440 machine, which contains the ARM compiler. Access the SENG440 machine through SSH in another terminal, and compile the decoding file using the following command:
```
arm-linux-gcc -static -o decode_optimized.exe decode_optimized.c
```

2. Once the executable is generated, upload the three files - the new executable, and 2 input files for the decoder - to the ARM machine using `lftp user<number>@arm`, where the user's `<number>` can be any number 1 through 4, and the password prompted for login is provided on the SENG440 Brightspace page. Within `lftp`, run the following command for each of the three required files:
```
put -O /sdcard/home/user4/ /tmp/decode_optimized.exe
```
This command assumes ARM's `/home/user4` is the target directory, and `/tmp/decode_optimized.exe` is the absolute path to the file in the SENG440 machine to be uploaded.

3. Login to the ARM machine via the command below:
```
telnet arm
```

4. Select the ARM device user to which the files were uploaded to, and login with proper credentials.

5. With the three files available in the default directory of the ARM user upon login, the Huffman decoder can be executed. Run the decoder against the test bench with the following command, where the lookup table `.lut` file is the first argument, and the Huffman-encoded file `.huf` is the second argument, as seen below:
```
./decode_optimized.exe input.txt.lut input.txt.huf
```

6. The decoder must complete its work and output a new file in the current directory called `decoded_output.txt`, which is the final decoded product of the lossless Huffman decoder. This file must match the original `input.txt` file from which the `.lut` and `.huf` files were generated initially.


## Acknowledgements
Thank you to our SENG440 professor, Mihai Sima, for answering the many questions our team had during the creation of this project and providing us with invaluable knowledge to assist the completion of this project.
