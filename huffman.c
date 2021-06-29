#include <stdio.h>
#include <string.h>

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
