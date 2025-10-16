
//Proj_7F_Stop_watch
 


//IT INTRODUCES

//1. Stop watch (SW) provided by the mini-OS

//2.  Project subroutines
  //I2C_Tx_Clock_command(one100ms_mode)       Initiates 100mS mode
  //I2C_Tx_Clock_command(ten_ms_mode)       Initiates 10mS mode
  //I2C_Tx_Clock_command(store_time)        Save time and pause display
  //I2C_Tx_Clock_command(display_stored_times)    Displays time saved to memory
  //I2C_Tx_Clock_command(display_current_time)    Resume stop watch mode
  

//USER INSTRUCTIONS

  //sw2 selects 100ms clock, sw1 a 10ms clock
  //Press sw2 to pause SW and save time
  //Press sw1 the read back saved times
  //Pressing sw2 to restore SW
  //Press sw3 to re-initialise SW

//Switch location SW1(PD2) - SW2(PD7) – SW3(PB2)


#include "Proj_7F_header_file_1.h"

volatile char T1_ovf_flag = 0;                           //requires volatile label
                                                         //even though "T1_ovf_flag" is used in "main"
                                                     
 





/**********************************************************************************************************/
ISR(TIMER1_OVF_vect) {TCCR1B = 0; T1_ovf_flag = 1;}       //stop timer 1 and set overflow flag



/**********************************************************************************************************/
ISR(PCINT0_vect) {if(switch_3_up){return;}
I2C_Tx_Clock_command(AT_exit_stop_watch);
while(switch_3_down);
SW_reset;}




/**********************************************************************************************************/
