
/*
 Program developed to check options for the reset function
Two EEPROM locations are used:

        MCUSR_copy      (0x3FC) This identifies POR and external reset and distinuishes beetween WDTouts due to the bootloader and user apps
        reset_ctl_reg   (0x3ED) This distinguishes between user SW resets, WDTouts at which a flag is set and WDTouts which trigger an interrupt
note:   All WDTouts trigger a jump to the bootloader (0x7000) except those which trigger an interrupt where the ISR is missing. These trigger a jump to 0x0000
 
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
case 5: Serial.write("\r\nProgram triggered by a WDTout with interrupt");break;
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
if((switch_2_up) && (switch_3_down)){while(1);}                                 //WDT times out with interrupt                                 

while(switch_1_down)wdr();                                                      //Wait for switch to be released

SW_reset;}



ISR (WDT_vect){Signal_WDTout_with_interrupt;wdt_enable(WDTO_15MS); }            //Optional




/*******************************************************************************************************************/
