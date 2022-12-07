
/*
 * 
 * EEPROM location ox3ED reserved for WDT reset source
 * 
 Raises number Num to a power Pow
 Num must be reduced to the form 1.A * 2^B 
 where 1.A is between 1 and 2 and 1.A * 2^B = Num

 A power series calculates the natural log of 1.A
 The natural log (ln) of 2^B is B *  ln 2
 These two results are added and multiplied by Pow
 
 The power series expansion of the exponential function
 is then used to obtain the final result (i.e the antilog)

See https://en.wikipedia.org/wiki/Exponential_function
and https://en.wikipedia.org/wiki/Natural_logarithm
for the power series deffinitions
*/

 
#include "Trig_function_header.h"

#define message_1 "\r\nTrig function: Enter angle in degrees\r\n"
#define message_2 "\r\n\r\nTime_out: Number too large or small. Try again!\r\n"



int main (void) 

{
char Num_string[12];

float Num, Num_bkp;                               //Scientfic number pus its backup
float Pow;
float Pie = 3.1415926;
int twos_exp;                                     //Power to which 2 is raised 
float Sine, Cos;                                       //The log of Num
float Log_result;                                 
float Result;
char expnt, Fn;
long  Denominator;
long FPN_digits;

setup_HW_Arduino_IO;

Serial.write(message_1);
Num = Sc_Num_from_PC(Num_string, '\t');           //User enters the scientific number
Num = Num *2.0 * Pie /360.0;
Sc_Num_to_PC (Num, 1,5,'\r');

Serial.write("Keypress c, s or t?");
Fn = waitforkeypress_A();

switch (Fn){
case 'c': Result = Cos_power_series(Num); Serial.write("Cos x =");break;
case 's': Result = Sine_power_series(Num);Serial.write("Sin x =");break;
case 't': Result = Sine_power_series(Num)/Cos_power_series(Num);Serial.write("Tan x =");break;}

Sc_Num_to_PC(Result,1,8,'\r');


FPN_digits = FPN_to_Significand(Result, &Denominator, &expnt);
FPN_digits = Fraction_to_Binary_Signed(FPN_digits, Denominator);
I2C_Tx_float_num(FPN_digits, expnt);
I2C_Tx_float_display_control;


SW_reset;
return 1; 
}




/**************************************************************************************************************************/
float Sine_power_series(float Num)

{float term;                                           //Power series terms
float difference;
float ans, ans_old;
long term_counter;
char sign = 0;

term = 1.0;
term_counter = 0;
ans = 0.0;
ans_old = 0.0;

while(term_counter <=100){
term_counter += 1;

term = (term * Num)/float(term_counter); 
if(!(term_counter%2))continue; 

if (((term_counter + 1)/2)%2)
ans += term;
else ans -= term;

if (term_counter -1){
difference = ans - ans_old;
if ((difference/ans > -0.0000001) && (difference/ans < 0.0000001))break;}
ans_old = ans;
}
return ans;}

//Sin x = x - (x^3)/6 + (x^5)/120.....etc.    






/**************************************************************************************************************************/
float Cos_power_series(float Num)

{float term;                                           //Power series terms
float difference;
float ans, ans_old;
long term_counter;
char sign = 0;

term = 1.0;
term_counter = 0;
ans = 1.0;
ans_old = 1.0;

while(term_counter <=100){
term_counter += 1;

term = (term * Num)/float(term_counter); 
if(term_counter%2)continue; 

if (((term_counter + 1)/2)%2)
ans -= term;
else ans += term;

if (term_counter -1){
difference = ans - ans_old;
if ((difference/ans > -0.0000001) && (difference/ans < 0.0000001))break;}
ans_old = ans;
}
return ans;}







/************************************************************************************************************************/
