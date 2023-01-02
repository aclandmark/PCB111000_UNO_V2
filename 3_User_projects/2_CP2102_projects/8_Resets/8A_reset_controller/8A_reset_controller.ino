
/*
 Program developed to check options for the reset function
 The bootloader reads the MCUSR (Micro controller status register) to identify resets due to POR and WDT.
 It uses writes to EEPROM to distinguish between WDT resets due to user programs and programming
 It also identifies program initiation following programming or pressing r at the p/r/t/D prompt.
 The reset control switch uses the WDT and EEPROM to trigger a SW reset.

Post reset the micro reads the EEPROM to determine its source.
 */

#include "8A_reset_controller_header.h"

int main (void){



setup_328_HW_Arduino_IO;


switch (reset_status){

case 1: User_prompt_Arduino;
Serial.write("\r\nProgram running following POR\t"); break;
case 2: Serial.write("\r\nProgram triggered by a SW_reset.\t"); break;
case 3: Serial.write("\r\nProgram triggered by the p/r/t/D... user prompt."); break;
case 4: Serial.write("\r\nProgram triggered by a flagged WDTout.\t"); break;
case 5: Serial.write("\r\nProgram halted (WDTout with ISR)\r\nPress reset ctl switch to continue\t");
Char_to_PC_as_binary((byte)copy_of_reset_ctl_reg);while(1)wdr();break;
case 6: Serial.write("\r\nWDTout. Press reset ctl switch to continue.");
Char_to_PC_as_binary((byte)copy_of_reset_ctl_reg);while(1)wdr();break;
case 7: Serial.write("\r\nJumps to 0x0000.\r\n(WDT configured with interrupt but ISR missing.)");}

Char_to_PC_as_binary((byte)copy_of_reset_ctl_reg);
_delay_ms(10);

set_up_PCI;
enable_PCI_on_sw1;

//cli();                          //This line prevents the fail safe mechanism from working
while(1)wdr();                    //Waits here for the switches to be operated 

return 1;
}


ISR(PCINT2_vect){ 
if (switch_1_up)return;
  sei();
  disable_PCI_on_sw1;
if((switch_2_up) && (switch_3_up));                                             //Followed by SW_reset
if((switch_2_down) && (switch_3_up)){Signal_flagged_WDTout;}                    //Followed by SW_reset
if((switch_2_up) && (switch_3_down))while(1);                                   //WDT times out with interrupt                                 
if((switch_2_down) && (switch_3_down)){cli();while(1);}                         //WDT times out but no interrupt is raised

while(switch_1_down)wdr();                                                      //Wait for switch to be released

SW_reset;}



//ISR (WDT_vect){Signal_WDTout_with_interrupt;WDTout;}      //Optional


void Char_to_PC_as_binary(unsigned char num){
Serial.write("\t\t");
for(int m = 0; m <= 7; m++)
{if (num & (1 << (7 - m)))Serial.write('1'); else Serial.write('0');}Serial.write("\t\t");}




/*******************************************************************************************************************/
