
#include "File_download_with_interrupt_header.h"

volatile long PORT_1 = 1;  
#define ctlz  0x1a

//Type main routine here

 




void T1_clock_tick(int T1_period_in_ms)                               //Start the T1 clock
{ TCNT1 = 0;
OCR1A = T1_period_in_ms * 125;
  TIMSK1 |= (1 <<  OCIE1A);
  TCCR1B = 0x03;
}



ISR(TIMER1_COMPA_vect) 
{
 TCNT1 = 0; 
 
if(PORT_1 < 0x10000)I2C_Tx_2_integers(PORT_1, PORT_1);
PORT_1 = PORT_1 << 1;
if (PORT_1 == 0x10000)PORT_1 = 1;}



/**************************************************************************************************************/
