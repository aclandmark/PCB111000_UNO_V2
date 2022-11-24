

/*
Takes a scientific number from the IO and converts it to the form A * 2^N 
where A is between 1 and 2 and N is an integer.

Press 
  SW1 to scroll through the digits
  SW2 to shift the display left
  SW3 to enter the number

Then press SW1 and SW3 to display A and N

Switch assignments:
    SW1 PORTD2 PCINT18 
    SW2 PORTD5 PCINT21
    SW3 PORTD7 PCINT23

    Note: if number exceeeds FPN bounds it is save as either zero or infinity
*/


 
#include "FPN_from_IO_header.h"

int main (void){

float FPN_1_num;
long ipart, Fnum_int;
int twos_exp;

setup_328_HW_Arduino_IO;

Data_Entry_complete=0;
clear_display;                       
FPN_1_num = FPN_number_from_IO();

Check_num_for_to_big_or_small(FPN_1_num);

if(FPN_1_num < 0){
_delay_ms(250);
FPN_1_num *= -1.0; 
float_num_to_display(FPN_1_num);}


if (FPN_1_num >= 1.0)                                           //Multiply or divide number by 2 until it
{twos_exp = 0; while (FPN_1_num >= 2.0)                         //is between 1 and 2 and adjust its twos_exp 
{FPN_1_num = FPN_1_num/2.0; twos_exp += 1;}}                    //so that its value remains unchanged 

if (FPN_1_num < 1.0)
{twos_exp = 0; while (FPN_1_num < 1.0)                          //Negative numbers not allowed
{FPN_1_num = FPN_1_num*2.0; twos_exp -= 1;}}

while(1){                                                       //Simple arithmetic
while(switch_1_up)wdr();                                        //Use reset control switch to exit
float_num_to_display(FPN_1_num);
while(switch_3_up)wdr();
Int_num_to_display(twos_exp);
while(switch_1_up)wdr();
float_num_to_display(pow(2, twos_exp) * FPN_1_num);
while(switch_3_up)wdr();}} 





/******************************************************************************************************************************/  
float FPN_number_from_IO(void){

char keypress = 0;
float Float_from_mini_OS;
float * Flt_ptr;
char * Char_ptr;

Flt_ptr = &Float_from_mini_OS;
Char_ptr = (char*)&Float_from_mini_OS;                            //Addresses FPN as four 8 bit character numbers

set_up_PCI;
enable_PCI_on_sw1_and_sw2;                                        //Required to scroll through the digits and shift the display left

initialise_display;

do{
while (!(digit_entry))wdr();                                       //Wait for user to select the next digit
digit_entry = 0;                                                  //SW2 sets this to one
}while(!(Data_Entry_complete));                                   //Remain in do-loop until data entry has been terminated
Data_Entry_complete = 0;

cr_keypress = 1;                                                  //Carriage return keypress: entry of FP string complete 
pause_PCI_and_Send_float_num_string;
cr_keypress = 0;

f_number_from_mini_OS;                                            //Mini-OS responds by displaying number and returning it in binary form

disable_PCI_on_sw1_and_sw2;
disable_PCI_on_sw3;

return Float_from_mini_OS;}



/****************************************************************************************************************************/
ISR(PCINT2_vect){
char disp_bkp[8];

if((switch_1_up) && (switch_2_up) && (switch_3_up))return;       //Take no action on switch release  
if ((switch_2_down) && (switch_3_down))
{while((switch_2_down) || (switch_3_down))wdr(); return;}        //Unwanted switch presses


/****Program control jumps to here when data entry is complete****************************************************************/
if(switch_3_down){                                              //SW3 is used to terminate data entry
digit_entry = 1;                                                //It is also used to generate decimal point
Data_Entry_complete=1;											                    //Signals to "FPN_number_from_IO()" that data entry is complete
pause_PCI_and_Send_float_num_string;							              //Update display
while(switch_3_down)wdr();                                      //Wait here for SW3 to be released
return;}

/*******Program control jumps to here during data entry**********************************************************************/
while(switch_1_down)
{scroll_float_display_zero();                                    //Disables IPC due to SW3 and scrolls through the available chars (0-9, E and -)
Timer_T2_10mS_delay_x_m(10);}

while(switch_3_down)wdr();                                       //Wait for SW3 to be released (may have been pressed to select decimal point)
enable_PCI_on_sw3;                                              //Will be required later on to terminate data entry              

if(switch_2_down)shift_float_display_left();                    //Press SW2 to accept the latest char and shift the display
Timer_T2_10mS_delay_x_m(10);
clear_PCI;} 





/************************************************************************************************************/
