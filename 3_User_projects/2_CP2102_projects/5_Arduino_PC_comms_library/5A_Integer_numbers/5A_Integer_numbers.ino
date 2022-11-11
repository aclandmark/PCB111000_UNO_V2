
/*
 From this point on Arduino library functions are introduced and will generally be used.
 For example 
 Serial.begin(115200):    This initialises the UART for 57600 Baud (assuming an 8MHz clock)
 Serial.write:            This sends text to the PC
 Serial print:            This sends numbers to the PC as text
 atol/atoi:               This converts a text string to a long / integer number
 ltoa:                    This converts a long number to text (askii characters)

 Note all characters received by the UART are saved to a buffer.
 Serial.readBytesUntil as implemented here reads these characters and transfers them
 to a separate buffer "num_as_string" stopping only when a cartridge return (cr) is detected
 or 20 keypresses have been made.
 The keypresses contained in the buffer "num_as_string" are then converted to a number. 
 

 This project accepts an integer number and prints out all the integer numbers by which it can be divided
 giving an integer result.
  */



#include "Integer_numbers_header.h"



/************************************************************************************************************/





int main (void)  
  { 
    char num_string[12];
    long  num;
    int m = 1;
 
 setup_328_HW_Arduino_IO;
    
   if (reset_status == 1) User_prompt;
 
   Serial.write("\r\nInteger number\t");
num = Int_Num_from_PC_local(num_string, '\r');

do{
if(!(num%m)){Int_Num_to_PC_local(m, num_string, '\t');}
m += 1; wdr();}while(m < num);

SW_reset;
return 1; 
 }





/******************************************************************************************/
long Int_Num_from_PC_local(char * num_as_string,char next_char)
{char strln;

pause_WDT;
Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, 20);
resume_WDT;
num_as_string[strln] = 0;
Serial.write(num_as_string);
Serial.write(next_char);
if(atol(num_as_string) > 0x7FFFF)
{Serial.write("\r\nNumber is too large\r\n"); SW_reset;}
return atol(num_as_string);}




/******************************************************************************************/
void Int_Num_to_PC_local(long Int_num, char * num_as_string, char next_char)
{
ltoa(Int_num, num_as_string, 10);
Serial.print(num_as_string);
Serial.print(next_char);
}






/******************************************************************************************/
