
//Introducing hex numbers


#include "Num_types_header_file.h"
#include "Local_subroutines.c"


char digits [12];

//Type main routine here


  

  
/**************************************************************************************************/
void Binary_to_PC_local(long x){ 
for (int m = 0; m<16; m++){
  if(x & (1 << (15-m))) Char_to_PC_Basic('1');  
  else Char_to_PC_Basic('0');   
  if(m%4==3)String_to_PC_Basic("  ");}}




/***************************************************************************************************/
void Long_Binary_to_PC_local(long x){ 
long x_high, x_low;
x_high = x >> 16;
x_low = x & 0xFFFF ;
 Binary_to_PC_local(x_high);
 String_to_PC_Basic("  ");
 Binary_to_PC_local(x_low);}



/*******************************************************************************************************/
