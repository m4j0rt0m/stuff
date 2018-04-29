///-----------------------------------------------------------------
///   Code:         bin2hex.c
///   Description:  Converts binary file into ascii hexadecimal format
///   Author:       m4j0rt0m https://github.com/m4j0rt0m
///   Date:         April 29 2018
///   Version:      1.0
///   Notes:        Should improve argument parsing
///-----------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

unsigned char hex_chars [16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

void chartohex(unsigned char char_input, unsigned char hex_output[]);

int main(int argc, char * argv[]) {
  /* declarations */
  FILE *input_file;                   //..input file pointer
  FILE *output_file;                  //..output file pointer
  int file_size;                      //..file size
  int line_size;                      //..line size
  unsigned char char_buffer = "";     //..char buffer
  unsigned char hex_buffer [2] = "";  //..hex buffer
  unsigned char * output_buffer;      //..output buffer
  int k;                              //..bytes count in buffer
  int index;                          //..buffer index

  /* arguments check */
  if(argc<3)  {
    printf("Usage: bi2hex <input_binary> <output_hex> <bytes_per_line>\n");
    return 0;
  }

  /* open input file */
  if((input_file = fopen(argv[1], "rb")) == NULL) {
    printf("Binary cannot be read...\n");
    return 0;
  }

  /* open output file */
  if((output_file = fopen(argv[2], "w")) == NULL) {
    printf("Cannot write output file...\n");
    return 0;
  }

  /* line size */
  line_size = strtol(argv[3], NULL, 10);  //..# of bytes per line
  output_buffer = (unsigned char *)calloc(line_size*2, sizeof(unsigned char));  //..allocate memory for buffer

  /* get size */
  fseek(input_file, 0L, SEEK_END);  //..file pointer at end of file
  file_size = ftell(input_file);    //..get size of the file in bytes
  fseek(input_file, 0L, SEEK_SET);  //..file pointer at end of file

  /* writes hex file */
  while(file_size = file_size - line_size){
    /* get output buffer */
    index = line_size*2;                      //..init index
    k = 0;                                    //..clear count
    for(int i=0; i<line_size; i++){
      char_buffer = fgetc(input_file);        //..read byte
      if(char_buffer == EOF){                 //..check if END OF FILE
        break;
      }
      chartohex(char_buffer, hex_buffer);     //..convert it to hex ascii
      output_buffer[index-1] = hex_buffer[0]; //..save lsb hex from byte
      output_buffer[index-2] = hex_buffer[1]; //..save msb hex from byte
      k += 2;                                 //..count one more read byte
      index -= 2;                             //..move index by 2
    }
    /* write output buffer */
    for(int i=0; i<k; i++) {
      fprintf(output_file, "%c", output_buffer[i]); //..print into output file whatever we read and converted
    }
    fprintf(output_file, "\n");                     //..insert a new line
  }

  /* end stuff */
  free(output_buffer);
  fclose(input_file);
  fclose(output_file);

  printf("Finished!\n");  //..finished...
  return 0;
}

/* convert byte to ascii hexadecimal value */
void chartohex(unsigned char char_input, unsigned char hex_output[]) {
  int ci_lsb, ci_msb;                 //..hex int
  ci_lsb = char_input & 0x0f;         //..get lsb
  ci_msb = (char_input>>4) & 0x0f;    //..get msb
  hex_output[0] = hex_chars[ci_lsb];  //..ascii hex for lsb
  hex_output[1] = hex_chars[ci_msb];  //..ascii hex for msb
}
