
/*
 Sines and cosines are calculated using series that have the same terms as those used by e^x. 
*/

 
#include "Trig_function_header.h"

#define message_1 "\r\nTrig function: Enter angle in degrees\r\n"

#define BL 30                                     //Buffer length


int main (void) 

{char Num_string[BL + 2];
float Num;                               //Scientfic number pus its backup
float Pie = 3.1415926;
float Sine, Cos;                                       //The log of Num
float Result;
char  Fn;

setup_HW_Arduino_IO;

if(!(watch_dog_reset))Serial.write(message_1);
else {watch_dog_reset = 0; Serial.write ("?\r\n");}
Num = Sc_Num_from_PC_A(Num_string, BL);           //User enters the scientific number
if(!(Num))Num = 0.0001;
else{
Num = Num *2.0 * Pie /360.0;}
Sc_Num_to_PC_A(Num, 1,5,'\r');

Serial.write("Keypress c, s or t?\r\n");
Fn = waitforkeypress_A();

switch (Fn){
case 'c': Result = Cos_power_series(Num); Serial.write("Cos x =");break;
case 's': Result = Sine_power_series(Num);Serial.write("Sin x =");break;
case 't': Result = Sine_power_series(Num)/Cos_power_series(Num);Serial.write("Tan x =");break;}

Sc_Num_to_PC_A(Result,1,8,'\r');

I2C_FPN_to_display(Result);

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
