
/*
  Raises number Num to a power Pow
 Num must be reduced to the form 1.A * 2^B 
 where 1.A is between 1 and 2 and 1.A * 2^B = Num

 A power series calculates the natural log of 1.A
 The natural log (ln) of 2^B is B *  ln 2
 These two results are added and multiplied by Pow
 
 The power series expansion of the exponential function
 is then used to obtain the final result (i.e the antilog)

 Note: ln 2 can also be obtained by summing a similar series of terms 

See https://en.wikipedia.org/wiki/Exponential_function
and https://en.wikipedia.org/wiki/Natural_logarithm
for the power series deffinitions
*/

 
#include "e_power_series_header.h"

#define message_1 "\r\nPower function: Enter +ve scientific number\r\n"
#define message_2 "\r\n\r\nTime_out: Number too large or small. Try again!\r\n"

#define BL 30                                     //Buffer length


int main (void) 

{
char Num_string[BL + 2];

float Num, Num_bkp;                               //Scientfic number pus its backup
float Pow;                                        //Power to which the number is to be raised
int twos_exp;                                     //Power to which 2 is raised 
float logN;                                       //The log of Num
float Log_result;                                 
float Result;


setup_HW_Arduino_IO;

if(!(watch_dog_reset))Serial.write(message_1);
if(watch_dog_reset){watch_dog_reset = 0; Serial.write(message_2);}

while(1){
Serial.write("?\r\n");
Num = Sc_Num_from_PC_A(Num_string, BL);           //User enters the scientific number

Sc_Num_to_PC_A(Num, 1,6, '\t');                 //Used to confirm operation of del key

Num_bkp = Num;

if (Num >= 1.0)                                   //Multiply or divide number by 2 untill it
{twos_exp = 0; while (Num >= 2.0)                 //is between 1 and 2 and adjust its twos_exp 
{Num = Num/2.0; twos_exp += 1;}}                  //so that its value remains unchanged 

if (Num < 1.0)
{twos_exp = 0; while (Num < 1.0)
{Num = Num*2.0; twos_exp -= 1;}}

Sc_Num_to_PC_A(Num, 1, 6, '\t'); 
Int_Num_to_PC_A(twos_exp, Num_string, '\r');

logN = logE_power_series(Num) + 
((float)twos_exp * 0.6931472);                     //Log to base e of the scientific number

Serial.write("Natural log is\t"); 
Sc_Num_to_PC_A(logN,1,5,'\r');

Serial.write("Enter power\t");
Pow = Sc_Num_from_PC_A(Num_string, BL);           //User enters the power.
Sc_Num_to_PC_A(Pow, 1,5, '\r');


Log_result = logN * Pow;                          //The Log of the result

Result = expE_power_series(Log_result);           //Returns the antilog

Serial.write("Local result is\t");

Sc_Num_to_PC_A(Result,1,5,'\r');
Serial.write("C function gives\t");                   //remove to save overwriting commentary
Sc_Num_to_PC_A((pow(Num_bkp,Pow)),1,5,'\r');          //remove to save overwriting commentary

I2C_FPN_to_display(Result);}

return 1;}




/**************************************************************************************************************************/
float expE_power_series(float Num)
{float term;                                           //Power series terms
float difference;
float ans, ans_old;
long term_counter;
char sign = 0;

One_Sec_WDT_with_interrupt;

if (Num < 0){sign = 1; Num = Num * (-1);}

term = 1.0;
term_counter = 1;
ans = 1.0;
ans_old = 1.0;

while(1){
term = (term * Num)/float(term_counter);  term_counter += 1;
ans += term;
difference = ans - ans_old;
if ((difference/ans > -0.0000001) && (difference/ans < 0.0000001))break;
ans_old = ans;}

setup_watchdog;

if(!(sign))return ans;
else return 1.0/ans;}

//e^x = 1 + x + (x^2)/2 + (x^3)/6 + (x^4)/24.....etc.     //Every increment is related to the previous one (i.e *x/(inc_num)




/**************************************************************************************************************************/
ISR (WDT_vect)
{wdt_enable(WDTO_30MS);while(1);}




/**************************************************************************************************************************/
float logE_power_series(float Num)
{float logE, logE_old;                                 //logs are calculated iteratively
float term;                                           //Power series terms
float difference;                                     //difference berween consequtive terms


Num -= 1;
if (Num > 0.9999){logE = 6.931472E-1;}
else if (Num < 0.00001){logE = 1E-6;}
else                                                  //Use power series to calculate the natural logarithm
{
int m = 1;
term = 1.0;
while(1){
term = term * Num/(float)m;
if (m == 1){logE = term; difference= logE;}
else{
  if (m%2)logE += term;
  else logE -= term;
  difference = logE - logE_old;}

if ((difference/logE > -0.0000001) && (difference/logE < 0.0000001))break; 
  
logE_old = logE;
term = term * (float)m;
if ((m += 1) > 5000)break; }}
return logE;}

//ln(1 + x) = x - (x^2)/2 + (x^3)/3....etc provided x is between 1 and 2.




/**************************************************************************************************************************/





/************************************************************************************************************************/
