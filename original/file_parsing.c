//main
int main(int argc, char *argv[]) {

   //set the locale to accept UTF-8 characters
   setlocale(LC_ALL, "");

   //check that a file was passed
   if(argc < 2) {
      printf("There was no file passed as argument to the program.\n");
      exit(1);
   }

   //open the file that was passed as argument
   FILE* fp = fopen(argv[1], "rb");

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

   //count the number of characters that need to be allocated in memory
   int char_count = 0;
   wchar_t c;

   while((c = getwc(fp)) != WEOF){

      //byte-swap to little-endian
      c = (c>>8) | (c<<8);

      //split all 1-byte UTF-8 chars into two separate characters
      if((c >> 8) != 0xC3){
         wchar_t c1 = (c >> 8);
         wchar_t c2 = (c & 0xff);
         printf("%x and %x\n", c1, c2);
         char_count = char_count + 2;
      }

      else{
         printf("A character outside 1-byte Unicode was found...");
      }

      //TODO: Handle French characters represented as 2-byte Unicode
      /*
      else{
         printf("%x\n", c);
         bytes = bytes + 2;
      }
      */
   }
   printf("File size:                %d bytes\n", bytes);
   exit(0);

   //allocate memory for the file string & read file
   wchar_t* file_string = malloc(bytes + 1); //add space for null terminator
   rewind(fp);
   fread(file_string, 1, bytes, fp);
   fclose(fp);
   
   file_string[bytes] = 0;
   //printf("%s\n", file_string);

   return file_string;