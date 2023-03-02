
/**************Proj_6L_Acquire_and process_FPN_Data_from_the KBD********************/

/*
 * Repeat of 6E_FPN to display but uses legacy routines
 * Acqures number in terms of the Significand in string form, a denominator and exponent
 * 
*/

#include "FPN_KBD_to_display_header.h"


int main (void){
float Num_1;
float power;
char digits[15];

long Significand;
long  Denominator;
char  expnt;
char counter = 0;


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

Significand = fpn_from_KBD_Legacy(digits, &expnt, &Denominator);
Num_1 = Significand_to_FPN((float)Significand, Denominator, expnt);

if(Num_1 > 0.0)power = 0.5;
else power = 3.0;

while (1){
  Int_Num_to_PC_A(counter,num_as_string, '\t');  
Sc_Num_to_PC_A(Num_1,1,6 ,'\r'); 

if (power == 0.5)
{if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down);}

if ((Num_1 < (1 + 5.0e-3)) && (Num_1 > (1 - 5.0e-3)))break;}

if (power == 3.0)
{if ((Num_1 < -1.0E20) || (Num_1 > -1.0e-20)){counter = 15; Num_1 *= -1.0; break;}
  I2C_FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter += 1;}

power = 1.0/power;
do{
Int_Num_to_PC_A(counter,num_as_string, '\t'); 
Sc_Num_to_PC_A(Num_1,1,6 ,'\r');                                            //Send number to PC

if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);


SW_reset;}




/************************************************************************************************/
