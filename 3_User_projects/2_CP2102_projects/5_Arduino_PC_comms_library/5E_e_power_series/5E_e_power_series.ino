
/*
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

 
#include "e_power_series_header.h"

#define message_1 "\r\nPower function: Enter +ve scientific number\r\n"
#define message_2 "\r\n\r\nTime_out: Number too large or small. Try again!\r\n"



int main (void) 

{
char Num_string[12];

float Num, Num_bkp;                               //Scientfic number pus its backup
float Pow;                                        //Power to which the number is to be raised
int twos_exp;                                     //Power to which 2 is raised 
float logN;                                       //The log of Num
float Log_result;                                 
float Result;

setup_328_HW_Arduino_IO;

if(reset_status == 2)Serial.write("?\r\n");
if(reset_status == 3)Serial.write(message_1);
if(reset_status == 5)Serial.write(message_2);

Num = Sc_Num_from_PC(Num_string, '\t');           //User enters the scientific number

Num_bkp = Num;

if (Num >= 1.0)                                   //Multiply or divide number by 2 untill it
{twos_exp = 0; while (Num >= 2.0)                 //is between 1 and 2 and adjust its twos_exp 
{Num = Num/2.0; twos_exp += 1;}}                  //so that its value remains unchanged 

if (Num < 1.0)
{twos_exp = 0; while (Num < 1.0)
{Num = Num*2.0; twos_exp -= 1;}}

Sc_Num_to_PC(Num, 1, 6, '\t'); 
Int_Num_to_PC(twos_exp, Num_string, '\r');



Reset_ATtiny1606;
logN = logE_power_series(Num) + 
((float)twos_exp * 0.693147);                     //Log to base e of the scientific number

Serial.write("Natural log is  "); 
Sc_Num_to_PC(logN,1,5,'\r');


Serial.write("Enter power  ");
Pow = Sc_Num_from_PC(Num_string, '\t');           //User enters the power.

Log_result = logN * Pow;                          //The Log of the result

Result = expE_power_series(Log_result);           //Returns the antilog

display_float_num_local(Result);
Sc_Num_to_PC(Result,1,5,'\r');
Serial.write("Library result\t");                   //remove to save overwriting commentary
Sc_Num_to_PC((pow(Num_bkp,Pow)),1,5,'\r');          //remove to save overwriting commentary

SW_reset;
return 1; 
}




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

if(!(sign))return ans;
else return 1.0/ans;
}

//e^x = 1 + x + (x^2)/2 + (x^3)/6 + (x^4)/24.....etc.     //Every increment is related to the previous one (i.e *x/(inc_num)




/**************************************************************************************************************************/
ISR (WDT_vect)
{Signal_WDTout_with_interrupt; 
wdt_enable(WDTO_30MS);while(1);}




/**************************************************************************************************************************/
float logE_power_series(float Num)
{float logE, logE_old;                                 //logs are calculated iteratively
float term;                                           //Power series terms
float difference;                                     //difference berween consequtive terms


Num -= 1;
if (Num > 0.9999){logE = 6.9315E-1;}
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
void display_float_num_local(float FP_num){
char * Char_ptr;

pause_pin_change_interrupt_on_PC5;
Char_ptr = (char*)&FP_num;
One_wire_Tx_char = 'D';                               //Command 'D' indicates that a floating point number will be sent
UART_Tx_1_wire();
for(int m = 0; m <= 3; m++){                          //Split the number into 4 chars
One_wire_Tx_char = *Char_ptr;                         //and send them individually
UART_Tx_1_wire(); 
Char_ptr += 1;}
reinstate_pin_change_interrupt_on_PC5;}







/************************************************************************************************************************/
