
/*Proj_7C_Basic_crystal_clock  A crystal controlled clock provided by the mini-OS.
******************************************************************************************/


/*IT INTRODUCES


Mini-OS clock command "I2C_Tx_OS_timer(AT_clock_mode, start_time)".
An AT clock mode"  of 7 starts the clok running.
The various modes (i.e. pause, hode etc.) are saved in the "clock_stop_watch .h" file.


USER INSTRUCTIONS

Following a reset we get the "R   R   R......" prompt. Press R and enter the time which is then saved to EEPROM.
With SW2 pressed it is now possible to transfer the project pcb to an independent 5V supply and the clock will automatically start 
at the time entered when SW2 is released.

There is a problem: Use of the EEPROM means that the pcb powers up in either one of two modes so that the pcb
can get out of step with what the user expects.

If time is not requested as expected press the reset button.

 */



#include "Proj_7C_header_file_1.h"



int main (void){
char User_response;
char digits[8], start_time[8];
unsigned char input_mode;
char keypress;

setup_HW_Arduino_IO;

if (switch_2_up)                                               //Press SW2 to omit user prompt
{User_prompt_A;}
while (switch_2_down);

input_mode = eeprom_read_byte((uint8_t*)0x02);                 //Read mode. Default value is 255

switch (input_mode){
case 255:                                                       //If EEPROM contains 255 initiate the clock
Serial.write("\r\nSend time: hours mins secs\
(24Hr clock)");
       
clear_display;
for (int m = 0;  m < 8; m++)
{                                                               //Obtain time from KBD            
keypress = waitforkeypress_A();

if((m == 2) ||(m == 5))
{digits[0] = ' ';save_to_eeprom;
shift_display_left; m += 1;}

digits[0] = keypress;
save_to_eeprom;
I2C_Tx_8_byte_array(digits);
if (m < 7){shift_display_left;}}

eeprom_write_byte((uint8_t*)(0x02),0);                         //Update mode and save in EEPROM location 2

Serial.write("\r\nFollow carefully (POR may be needed)!!\r\nPress SW2 & Power cycle!:\
Can now use 5V USB charger if required.\r\n\
Release SW2 ro restart clock at time entered.\r\n");

break;                                                        //Exit and send Start clock command

case 0:                                                       //If EEPROM location 2 contains 0 start clock immediately
eeprom_write_byte((uint8_t*)(0x02),255);                      //Restore the EEPROM location 2 to its default value
break;

default: eeprom_write_byte((uint8_t*)(0x02),255);             //If EEPROM ever gets corrupted reset it to 255  (0b11111111)
wdt_enable(WDTO_15MS); while(1);break;}                       //Exit and start clock          

for (int m = 0;  m < 8; m++)
{start_time[m] = 
eeprom_read_byte((uint8_t*)(m+3));}

I2C_Tx_OS_timer(AT_clock_mode, start_time);                  //Send Start clock command (AT clock mode is 7)

while(1);}  



/***********************************************************************************************************************/
