

//3 projects that explore the conversion of strings to numbers so that
//they can be entered at the keyboard





#include "Receiver_Transmitter_header.h"


//Three examples to experiment with




/***********************Subroutines*********************************/
void Num_string_from_KBD_Local(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    Char_to_PC_Basic(keypress);
    array_ptr += 1;
  }
  *array_ptr = '\0';}



/********************************************************************************************************/
long Askii_to_binary_Local(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';
    }
  }
  return num;}




/********************************************************************************************************************/
