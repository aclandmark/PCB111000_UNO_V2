
/*
 Program developed to check options for the reset function
 The bootloader reads the MCUSR (Micro controller status register) to identify resets due to POR and WDT.
 It uses writes to EEPROM to distinguish between WDT resets due to user programs and programming
 It also identifies program initiation following programming or pressing r at the p/r/t/D prompt.
 The reset control switch uses the WDT and EEPROM to trigger a SW reset.

Post reset the micro reads the EEPROM to determine its source.
 */

#include "8A_header_file_1.h"

int main (void){

setup_HW_with_reset_analysis;


switch (reset_status){

case 1: User_prompt;
Serial.write("\r\nProgram running following POR\t"); break;
case 2: Serial.write("\r\nProgram triggered by a SW_reset.\t"); break;
case 3: Serial.write("\r\nProgram triggered by external reset or run (the -r- keypress)."); break;
case 4: Serial.write("\r\nProgram triggered by a flagged WDTout.\t"); break;
case 5: Serial.write("\r\nProgram triggered by a WDTout with interrupt\r\n");break;
case 6: Serial.write("\r\nJumps to 0x0000 rather than 0x7000\r\n(WDT configured with interrupt but ISR missing.)");break;}


set_up_PCI;
enable_pci_on_sw1;
while(1)wdr();                    //Waits here for the switches to be operated 
return 1;
}


ISR(PCINT2_vect){ 
if (switch_1_up)return;
  sei();
  disable_pci_on_sw1;
if((switch_2_up) && (switch_3_up));                                             //Followed by SW_reset
if((switch_2_down) && (switch_3_up)){Signal_flagged_WDTout;}                    //Followed by SW_reset
if((switch_2_up) && (switch_3_down)){while(1);}                                   //WDT times out with interrupt                                 
if((switch_2_down) && (switch_3_down)){cli();while(1);}                         //WDT times out but no interrupt is raised

while(switch_1_down)wdr();                                                      //Wait for switch to be released

SW_reset;}



//ISR (WDT_vect){Signal_WDTout_with_interrupt;wdt_enable(WDTO_15MS); }            //Optional




/*******************************************************************************************************************/
