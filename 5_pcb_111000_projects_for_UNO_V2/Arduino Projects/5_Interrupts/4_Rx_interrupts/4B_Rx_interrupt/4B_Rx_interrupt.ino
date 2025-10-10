
volatile char sym_counter;

#include "4B_Rx_interrupt_header.h"


//Type main here






ISR(USART_RX_vect){
Char_from_PC_Basic();
newline_Basic();
for (int m = 0; m <= sym_counter; m++)String_to_PC_Basic(" ");}




/*******************************************************************/
