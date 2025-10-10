

      //Six examples using a timer interrupt to drive the display
      //Take special care with examples 5 an 6: main routine has one extra line

#include "Interrupt_basic_header.h"

volatile long PORT_1 = 1, PORT_2 = 0x8000;                            //For example 1, 2 and 3
volatile int m = 0, n = 0, n_max;                                     //Extras for example 3
volatile unsigned int clock_rate = 150;                               //Extra for examples 2 and 3
volatile unsigned int PRN;                                            //For example 4
unsigned char PRN_counter = 0;                                        //For example 4
volatile unsigned char counter = 0;                                   //For example 4



 int main (void)   
  {
  setup_HW;
set_up_pci_on_sw1_and_sw2;                                             //Eamples 2 and 3 only
enable_up_pci_on_sw1_and_sw2;                                              //Eamples 2 and 3 only
    sei();
   T1_clock_tick(clock_rate);
   //initialise_display();                                              //Examples 5 and 6 ONLY
while(1);
   SW_reset;
    }



//*****************************************************************************************************
void T1_clock_tick(unsigned int T1_period_in_ms)              //Sets up the T1 clock (default value of T1_period_in_ms is 150) 
{ TCNT1 = 0;                                                  //TCNT1 increases by 1 for each clock tick 
OCR1A = T1_period_in_ms * 125;                                //Set register OCR1A to 150 x 125 = 18750
  TIMSK1 |= (1 <<  OCIE1A);                                   //Generates an intrerrupt TCNT1 equals OCR1A (i.e. after 150mS) 
  TCCR1B = 0x03;}                                             //Starts the clock with a speed of 125 ticks per mS



//*****************************************************************************************************
ISR(PCINT2_vect) {                                                      //Use with examples 2 & 3 only
  if ((switch_3_down)|| (clock_rate <= 10))clock_rate = 150;
  else
  {if (switch_1_down)clock_rate += 20;
  if (switch_2_down)clock_rate -= clock_rate/7;}}



//*****************************************************************************************************
void initialise_display()
{ n = 1;
  PORT_1 = 1;
  PORT_2 = 0x8000;
   n_max = 16;
  
 I2C_Tx_2_integers(PORT_1, PORT_2);}



//*****************************************************************************************************
//Type Timer1 ISR here

  
  
 





//********************************************************************************************************
