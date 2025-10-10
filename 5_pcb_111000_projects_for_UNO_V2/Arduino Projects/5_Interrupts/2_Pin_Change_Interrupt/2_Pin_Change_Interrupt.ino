
// Target practice:
// In this example a basic LED driver routine runs in the for ground.
// The user operates user switch SW2 to shoot the LEDs down.
 
// Note SW2 is connected to a pin on the Atmega328 that is configured as input.
// When SW2 is pressed the voltage level on the pin changes from 5V to zero volts.
// A Pin Change Interrupt (PCI) is generated.
// 


#include "Pin_Change_Interrupt_header.h"

unsigned int PORT_1;                                    //Controls display
char scan;                                              //Controls led scan       
char switch_control;                                    //Temporarily disables switch
volatile unsigned int mask;                             //Records which leds haver been shot down


//Type main routine here.





ISR(PCINT0_vect)
{ if (switch_3_up)return;                             //No action when switch is released
  if (switch_control) return;                         //Switch disabled: No action required
  if (mask & PORT_1)                                  //Led shot down                                 
  { mask &= ~PORT_1;                                  //Update mask to record which one
    scan = 0;                                         //Led shot down: Pause scan
  }
  switch_control = 1;                                //Switch pressed: Disable switch
}





/***********************************************************************************************************/
