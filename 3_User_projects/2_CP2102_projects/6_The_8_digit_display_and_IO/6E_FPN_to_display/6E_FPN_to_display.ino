
/**************Proj_5E_Acquire_and process_FPN_Data_from_the _KBD********************/

/*
Like 5D but processes floating point data

Use the term Significand in place of FPN_digits

Does NOT send 100,100,1000 10000 etc correctly to the display
ut does acquire them correctly!!

*/



#include "FPN_KBD_to_display_header.h"


int main (void){
float Num_1;
float power = 0.5;
char digits[15];

long Significand;
long  Denominator;
char  expnt;
char counter = 0;


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number_2 \
& terminate with Return key.\r\n");

Significand = fpn_from_KBD(digits, &expnt, &Denominator);
Num_1 = Significand_to_FPN((float)Significand, Denominator, expnt);

while (1){
Int_Num_to_PC(counter,num_as_string, '\t');  
Sc_Num_to_PC(Num_1,1,6 ,'\r');                            //Send number to PC

if (!(counter%5))
{Significand = FPN_to_Significand(Num_1, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;

while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter += 1;                                //Do some arithmetic
if ((Num_1 < (1 + 5.0e-3)) && (Num_1 > (1 - 5.0e-3)))break;}

power = 1.0/power;
do{
Int_Num_to_PC(counter,num_as_string, '\t'); 
Sc_Num_to_PC(Num_1,1,6 ,'\r');                            //Send number to PC

if (!(counter%5))
{Significand = FPN_to_Significand(Num_1, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;

while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);

SW_reset;}
