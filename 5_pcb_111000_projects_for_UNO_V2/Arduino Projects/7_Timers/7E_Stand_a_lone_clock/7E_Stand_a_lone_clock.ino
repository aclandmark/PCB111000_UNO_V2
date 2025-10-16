//Proj_7E_Stand_a_lone_clock. A development of Proj_7D


//IT INTRODUCES

//A clock that can be setup and adjusted completely independently of a PC and uses the 
//crystal controlled timer provided by the mini-OS.


//OPERATION

//Following POR or programming, PCI is active and switches 1 and 3 are used to enter the start time
//and start the clock.
//PCI is then disabled and program execution polls the three switches in turn waiting 
//for the user to select either pause or blank the display or adjust the time.



//USER INSTRUCTIONS

//Reset with sw1 down to print user instructions

//Power cycle or reset the project pcb.
//A cursor indicates the active digit. Pulse SW1 to set it.
//Pulse SW2 to set the next digit
//When display is complete pulse sw2 to start the clock


//During operation the three switches are continuously polled
//Pulse sw2 to pause or restart the display           
//Pulse sw1 to toggle the display ON and OFF

//To adjust the clock:
//To advance it
//Pulse sw3 then press sw1 for minutes and sw1 for seconds. Pulse sw3 when done.

//To retard it
//Press and hold sw3 (for >= 500ms)
//Press sw2 for minutes and sw1 for seconds. Pulse sw3 when done.

//Switch location SW1(PD7) - SW2(PB6) – SW3(PD2)

#define message_1 \
"\r\nCRYSTAL CLOCK with adjustment\r\n\
Press sw1 to toggle the display on and off\r\n\
Press sw3 to pause or resume the clock\r\n\
To adjust the clock:\r\n"

#define message_2 \
"pulse sw2 then press sw1 and sw3 to advance the time\r\n\
or press and hold sw2 (for 500ms)\r\n\
then press sw1 and sw3 to retard the time\r\n\
Always pulse sw2 when the time is correct\r\n"

#define User_instructions \
Serial.write(message_1);\
Serial.write(message_2);

#include "Proj_7E_header_file_1.h"


volatile int digit_num = 7;
char start_time[8];
volatile char Data_Entry_complete=0;


//Type main routine here




/******************************************************************************************************************************/
void time_from_IO(void){
char temp=0;
set_up_pci;
enable_pci;

disable_pci_on_sw3;
for(int m = 0; m<= 7; m++)start_time[m] = 0;
start_time[7] = '_';I2C_Tx_8_byte_array(start_time);
sei();
while(!(Data_Entry_complete));                                                 //Line A. wait here for pci interrupts used to enter data
Data_Entry_complete = 0;        
for(int m = 0; m<=3; m++)                                                      //Reverse "start_time" array
{temp = start_time[m]; 
start_time[m] = start_time[7-m];
start_time[7-m] = temp;}}



/******************************************************************************************************************************/
ISR(PCINT2_vect) {                                                             //input number: store keypresses in array -start_time
if((switch_1_up) && (switch_2_up))return;
Timer_T0_10mS_delay_x_m(5);                                                   //Place delay here to trap bounce on sw release
clear_pci_on_sw1_and_sw2;

if (switch_1_down){                                                            //Switch_2: used to scroll through the permissible                    
switch(digit_num){                                                             //characters for each digit on the display
case 7: 
switch (start_time[7]){                                                       //Enter Digit 7 hours 0,1 or 2
case '0': case '1': start_time[7]++; break;
case '2': start_time[7] = '0';break;
default:start_time[7]='0';break; }break;

case 6:
if (start_time[7]== '2'){                                                     //Enter Digit 6 hours 0,1,2 or 3
switch (start_time[6]){
case '0': case '1': case '2': start_time[6]++; break;
case '3': start_time[6] = '0';break;
default:start_time[6]='0';break;}} 

else                                                                          //Enter Digit 6 hours 0 to 9
switch (start_time[6]){
case '_': case '9': start_time[6] = '0'; break;
default:start_time[6]++;break;}
break;

case 4:                                                                       //Enter Digit 4 minutes 0 to 5
switch (start_time[4]){
case '0': case '1': case '2':
case '3': case '4': start_time[4]++; break;
case '5': start_time[4] = '0'; break;
default:start_time[4]='0';break;} break;


case 3:                                                                      //Enter Digit 3 minutes 0 to 9
switch (start_time[3]){
case '_': case '9': start_time[3] = '0'; break;
default:start_time[3]++;break;} break;


case 1:                                                                      //Enter Digit 1 seconds 0 to 5
switch (start_time[1]){
case '0': case '1': case '2':
case '3': case '4': start_time[1]++; break;
case '5': start_time[1] = '0'; break;
default:start_time[1]='0';break;} break;

case 0:                                                                      //Enter Digit 0 secondes 0 to 9
switch (start_time[0]){
case '_': case '9': start_time[0] = '0'; break;
default:start_time[0]++;break;} break;}

I2C_Tx_8_byte_array(start_time);}                                           //Keep display up-to date 
      


if (switch_2_down){                                                         //Switch_1: Used to select a character and move onto the next digit
switch(digit_num){
case 7: case 4: case 1:
digit_num--;start_time[digit_num] = '_';break;
case 6: case 3: digit_num -=2; start_time[digit_num] = '_'; break;
case 0: digit_num = 7; Data_Entry_complete = 1;
disable_pci_on_sw1_and_sw2;
start_time[2] = ' ';
start_time[5] = ' ';
while(switch_2_down);Timer_T0_10mS_delay_x_m(10); 
break;}
I2C_Tx_8_byte_array(start_time);}}                                           //return to Line A when sw1 is released


/*******************************************************************************************************************************/
