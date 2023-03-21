
/*
 * 0.99^0.05 and 0.99^-0.05 OK
 * Reducing the exponent causes a failure
 * Do notinput data thatexceed FPN bounds
 * Use of del key when entering data does not function
 */

 
#include "e_power_series_header.h"

#define message_1 "\r\nPower function: Enter +ve scientific number\r\n"
#define message_2 "\r\n\r\n"
#define Buff_Length  20

int main (void) 
{

char Num_string[Buff_Length + 2];
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
Num = Sc_Num_from_PC_A(Num_string, Buff_Length);                     //User enters the scientific number (back space is not cattered for)
Num_bkp = Num;

if(FPN_GT_or_EQ(Num, 1.0))                                  //Multiply or divide number by 2 untill it
{twos_exp = 0; while (FPN_GT_or_EQ(Num, 2.0))               //is between 1 and 2 and adjust its twos_exp
{Num = FPN_div(Num, 2.0); twos_exp += 1;}}                 //so that its value remains unchanged

if (FPN_LT(Num, 1.0))
{twos_exp = 0; while (FPN_LT(Num, 1.0))
{Num = FPN_mult(Num, 2.0); twos_exp -= 1;}}

FPN_to_String(Num, 1, 6, '\t',Num_string);
Serial.write (Num_string);
Int_Num_to_PC_A(twos_exp, Num_string, '\r');                  //Echo data to screen

logN = logE_power_series(Num); 
if(twos_exp) logN 
+= FPN_mult((float)twos_exp, 0.6931472);                     //Log to base e of the scientific number

Serial.write("Natural log is  "); 
FPN_to_String(logN, 1, 5, '\r',Num_string);
Serial.write (Num_string);


Serial.write("Enter power  ");
Pow = Sc_Num_from_PC_A(Num_string, Buff_Length);                     //User enters the power.

Log_result = FPN_mult (logN, Pow);                          //The Log of the result

if((FPN_LT (Log_result, 0.0005)) 
&& (FPN_GT (Log_result, -0.0005)))
{Serial.write("Unity."); SW_reset;}                         //Result very close to one.

Result = expE_power_series(Log_result);                     //Returns the antilog
FPN_to_String(Result, 1, 5, '\r',Num_string);
Serial.write (Num_string);

Serial.write("Library result\t");                           
Sc_Num_to_PC_A((pow(Num_bkp,Pow)),1,5,'\r');      

I2C_FPN_to_display(Result);}

return 1;}




/**************************************************************************************************************************/
float expE_power_series(float Num)
{float term;                                           //Power series terms
float difference;
float ans, ans_old;
long term_counter;
char sign = 0;

if ((FPN_GT (Num, 85.0)) 
|| (FPN_LT (Num, -85.0)))
{Serial.write("~Exceeds FPN bounds."); SW_reset;}
if (FPN_LT(Num, 0))
{sign = 1; Num = FPN_mult (Num, (-1));}

term = 1.0;
term_counter = 1;
ans = 1.0;
ans_old = 1.0;

while(1){
term = FPN_mult(term, FPN_div(Num, (float)term_counter)); 
term_counter += 1;

ans = FPN_add(ans, term);   
difference = FPN_sub (ans, ans_old);

if((FPN_GT(FPN_div(difference, ans), -1e-5)) 
&& (FPN_LT(FPN_div(difference, ans), 1e-5)))break;

ans_old = ans;}

setup_watchdog;

if(!(sign))return ans;
else return FPN_div(1.0, ans);}

//e^x = 1 + x + (x^2)/2 + (x^3)/6 + (x^4)/24.....etc.     //Every increment is related to the previous one (i.e *x/(inc_num)




/**************************************************************************************************************************/
float logE_power_series(float Num)
{float logE, logE_old;                                  //logs are calculated iteratively
float term;                                             //Power series terms
float difference;                                       //difference berween consequtive terms

if (FPN_GT(Num, 1.99))return 0.693;
Num = FPN_sub (Num, 1.0);
if (Num < 0.00001){logE = 1E-5;}
else                                                    //Use power series to calculate the natural logarithm
{
int m = 1;
term = 1.0;
while(1){
term = FPN_mult(term, FPN_div(Num, (float)m));

if (m == 1){logE = term; difference= logE;}
else{
  if (m%2)logE = FPN_add (logE, term);
  else logE = FPN_sub(logE, term); 
  difference = FPN_sub(logE, logE_old); }

if((FPN_GT(FPN_div(difference, logE), -1e-5)) 
&& (FPN_LT(FPN_div(difference, logE), 1e-5)))break;
 if (!(m%20))Serial.write('.'); 
 
logE_old = logE;
term = FPN_mult(term, (float)m);
if ((m += 1) > 500)break; }}
return logE;}

//ln(1 + x) = x - (x^2)/2 + (x^3)/3....etc provided x is between 1 and 2.







/**************************************************************************************************************************/
