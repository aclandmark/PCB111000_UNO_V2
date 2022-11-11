
/*
 Accepts an integer number and converts it to its prime factors
 */




#include "Prime_factors_header.h"



/************************************************************************************************************/





int main (void)  
  { 
    char num_string[12];
    long  number;
    long factor;
    char factor_counter;
 
 setup_328_HW_Arduino_IO;
    
   if (reset_status == 1) User_prompt;                            //Only required for a POR
 
   Serial.write("\r\nInteger number\t");
   number = Int_Num_from_PC(num_string, '\r');
  
  factor_counter = 0;
  do{
  factor = Product_search(number);                                 //Get lowest factor
  number = number/factor;                                          //Next number to factorise
  Int_Num_to_PC(factor,num_string, '\t' );
  factor_counter += 1;} 
  while (number != 1);  

  if(factor_counter == 1){Serial.write("Prime\r");}
  SW_reset;
  return 1;}



/************************************************************************************************************/
long Product_search  (long number)  {
long search_array[200]; 
int j=0;
long n=200;
long L=0;

while(1){
for(long k =0; k < n; k++)                              //Populate array (i.e. with 401, 402, 403, ..etc) 
{search_array[k] = k+1+n*L;}
prime_no_generator(L,n,search_array);                   //Set all non primes in array to zero
j=0; 
while(search_array[j] == 0)j += 1;                     //Select first prime number
while (j<=200)
{if ((search_array[j] >= 2) &&
(number%search_array[j] == 0))                         //Scan array until a factor is found
{return search_array[j];}                              //and return it
j += 1;} 
if(L*200 > number)return 0;                            //No factors found at all: Return zero for prime number
L += 1; Serial.write('.');}}                           //No factor found in array: increment L and populate new array



/************************************************************************************************************/
void prime_no_generator
(long L, long n,  long reg[]){                  //n the array size is 200, I is the array number (zero for the first one)
  
long i;                                         //We compute the product i * m
long m;                                        //i always starts at 2 then increments to 3, 4, etc.
i=2;                                           //m is set to take us to the beginning of the array 
while (i*i < n*(L+1)){                         //Maximum value i can take
wdr();
m=0;
while(i*(i+m) <= n*L)m++;                      //Go to start of the new array for example
while(i*(i+m) <= n*(1+L))                      //2*101, 2*102, 2*103,... etc   3*67,3*68, 3*69,...etc    5*41, 5*42,...etc
{reg[i*(i+m) -1 - n*L] = 0; m++;wdr(); }      //set appropriate registers to zero (Note the offset of 1 place) 
i++;}}




/************************************************************************************************************/
