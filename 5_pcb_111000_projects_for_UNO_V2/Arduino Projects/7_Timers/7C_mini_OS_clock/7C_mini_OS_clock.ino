
//Proj_7C_Basic_crystal_clock  A crystal controlled clock provided by the mini-OS.


//IT INTRODUCES


//Mini-OS clock command "I2C_Tx_OS_timer(AT_clock_mode, start_time)".
//An AT clock mode of 7 starts the clok running.
//The various modes (i.e. pause, hode etc.) are saved in the "clock_stop_watch .h" file.


//USER INSTRUCTIONS

//Following a reset we get the "R   R   R......" prompt. Press R and enter the time which is then saved to EEPROM.
//With SW2 pressed it is now possible to transfer the project pcb to an independent 5V supply and the clock will automatically start 
//at the time entered when SW2 is released.

//There is a problem: Use of the EEPROM means that the pcb powers up in either one of two modes so that the pcb
//can get out of step with what the user expects.
//A reset with SW3 pressed will reset the EEPROM

//If time is not requested as expected press the reset button.





#include "Proj_7C_header_file_1.h"


char  start_time[8];
unsigned char input_mode;
char keypress;


//Enter main routine here 



/***********************************************************************************************************************/
