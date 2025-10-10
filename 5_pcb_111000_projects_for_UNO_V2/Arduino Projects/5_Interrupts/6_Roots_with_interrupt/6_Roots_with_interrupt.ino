

#include "Roots_with_interrrupt_header.h"
#include "Local_subroutines.c"

void Num_string_from_KBD_Local(char *);

#define message_1 "\r\nThis program calculates roots.\r\n\
Enter integer number\r\n?"


volatile long PORT_1 = 1, PORT_2 = 0x8000;  
volatile char num_present = 0;                    //Set to one when number has been entered and -cr- presses (See Local subroutines)
volatile char ready_to_compute = 0;               //Set to one as the ISR exits (to provide a fixed known time for calculations)
long Num;
char Num_string[12];



//Type main routine here





/********************************************************************************************************/
void T1_clock_tick(int T1_period_in_ms)                               //Start the T1 clock
{ TCNT1 = 0;
  OCR1A = T1_period_in_ms * 125;
  TIMSK1 |= (1 <<  OCIE1A);
  TCCR1B = 0x03;}



/********************************************************************************************************/
ISR(TIMER1_COMPA_vect) 
{
 TCNT1 = 0; 
     I2C_Tx_2_integers(PORT_1, PORT_2);
      PORT_1 = PORT_1 << 1;
      PORT_2 = PORT_2 >> 1;
   if (PORT_1 == 0x10000){PORT_1 = 1; PORT_2 = 0x8000;} 
 if (num_present) {ready_to_compute = 1;} }



/********************************************************************************************************/
long Askii_to_binary_Local(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';}}
  return num;}






  /*************************************************************************************/
