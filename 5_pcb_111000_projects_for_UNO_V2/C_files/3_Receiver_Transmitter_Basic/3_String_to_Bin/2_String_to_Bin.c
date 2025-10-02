

#include "Local_subroutines.c"

int main (void){            		//Example 3
  char num_string[12];				//Re-oders numbers smallest to largest
  int num_array[20],  array_length=0;
      
setup_HW_basic;

String_to_PC_Basic("Enter numbers at keyboard (zero to exit)\r\n");

while(1){
Num_string_from_KBD_Local(num_string);
Char_to_PC_Basic(' ');
num_array[array_length] = Askii_to_binary_Local(num_string);
if(!(num_array[array_length++]))break;}
newline_Basic();
array_length -= 1;

Re_order_array(num_array,array_length);

SW_reset;
return 1;}