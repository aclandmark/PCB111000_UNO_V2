
//Defining digits in terms of their segments
//so that numbers can simply be entered at the keyboard


#include "Segment_driver_header.h"
#include "Local_subroutines.c"


#define zero "abcdef"                   //chars a,b,c,d,e and f are stored in an array named "zero"
#define one "bc"                        //chars b and c are stored in an array named "one"
#define two "abdeg"                     //Note: the compiler terminates each string in zero
#define three "abcdg"
#define four "bcfg"
#define five "acdfg"
#define six "gcdef"
#define seven "bca"
#define eight "abcdefg"
#define nine "gabcf"

const char *message_1 = "String memory dump\t";
  const char *message_2 = "\r\nDigit\tAddress    String\r\n";


//Enter main routine here





/************************************************************************************************************************/
void display_num_string (const char* s, int digit_num){             //Subroutine requires a pointer to the string   
int char_ptr=0;                                                     //containing segments used to define a digit
char letter;

while(1){
letter = *(s + char_ptr);// (s[char_ptr]);                          //Note these two expressions are equivalent
switch(letter){                                                     //Work through the segments contained in the 
case 'a':                                                           //string until '\0' is encountered
case 'b': 
case 'c': 
case 'd': 
case 'e': 
case 'f': 
case 'g': I2C_Tx_any_segment(letter, digit_num);
break;                                                              //update display one segment at a time
case 0:  return; break;                                             //zero indicates the end of the string
default: break;}char_ptr++;}}                                       //incrementing "char_ptr" steps through the string
                                                                    //Selecting segment letters in turn



/************************************************************************************************************************/ 
