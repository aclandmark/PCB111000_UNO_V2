

//Entering numbers from the keyboard and letting the mini-OS convert them to binary 

#include "Receiver_Transmitter_header.h"

int main (void){   
  char num_string[12];
  long num;
  
setup_HW;
if(watch_dog_reset == 1)String_to_PC_Basic("\r\nAgain"); else
String_to_PC_Basic("Enter number at keyboard");
num = Int_KBD_to_display_A_Local(num_string);
waitforkeypress_Basic();
while(num){
num /= 2;
I2C_Tx_long(num);
waitforkeypress_Basic();}
SW_reset;
return 1;}




/************************************************************************/
//Type subroutine 
//long Int_KBD_to_display_A_Local(char *) here






/******************************************************************************/
