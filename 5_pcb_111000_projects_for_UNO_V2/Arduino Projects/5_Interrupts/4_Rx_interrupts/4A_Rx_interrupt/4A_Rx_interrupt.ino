

volatile int p=0;

char repeats[20];

#include "4A_Rx_interrupt_header.h"


//Type main here





ISR(USART_RX_vect)
{ repeats[p++] = Char_from_PC_Basic();}




/***************************************************************************************/
