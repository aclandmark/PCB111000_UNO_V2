
/**************Proj_6E_Acquire_and process_FPN_Data_from_the KBD********************/

/*
 * PRINTS column of numbers each one the square root of the previous
 * until within 0.005 of unity then continues but calculates the squares
 * 
*/

#include "FPN_KBD_to_display_header.h"


int main (void){
float Num_1;
float power = 0.5;
char digits[15];

char counter = 0;


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

Num_1 = fpn_from_KBD(digits); //digits, &expnt, &Denominator);

while (1){
Int_Num_to_PC(counter,num_as_string, '\t');  
Sc_Num_to_PC(Num_1,1,6 ,'\r'); 

if (!(counter%5)){FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter += 1;                                //Do some arithmetic
if ((Num_1 < (1 + 5.0e-3)) && (Num_1 > (1 - 5.0e-3)))break;}

power = 1.0/power;
do{
Int_Num_to_PC(counter,num_as_string, '\t'); 
Sc_Num_to_PC(Num_1,1,6 ,'\r');                            //Send number to PC

if (!(counter%5)){FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);

SW_reset;}




/************************************************************************************************/
