
#include "Prime_numbers_header.h"


   
 //Type main routine here  




/******************************************************************************************************************/
long next_prime_number(long *pointer, long *reg, int array_size){/////////////////////////////////
  
  while(!(*(reg + *pointer)))*pointer = *pointer +1;

  if (*pointer == array_size)return 0;
  return ((long)*(reg + (*pointer)++));
  }

/***************************************************************************************************************************/
long prime_no_generator                        
(long I,long n,  long reg[]){            //n the array size is 200, I is the array number (zero for the first one)

long i=2;                                       //We compute the product i * m
long m;                                         //i always starts at 2 then increments to 3, 4, etc. 

for(int j =0; j < n; j++)              //Populate an array with a sequence of numbers 
{reg[j] = j+1+(n*I);}                  //for example 201, 202, 203, .....400
                                                

while (i*i < n*(I+1)){                         //Maximum value i can take
wdr();

if (I)m = (n*I/i)-i +1;
else m = 0;

while(i*(i+m) <= n*(1+I))                       //2*101, 2*102, 2*103,... etc   3*67,3*68, 3*69,...etc    5*41, 5*42,...etc
{reg[i*(i+m) -1 - n*I] = 0;                   //set appropriate registers to zero (Note the offset of 1 place).
while((!(reg[i*(i+m) -1 - n*I]))
&& ((i*(i+m) -1 - n*I) < n))
{m+=1;}}

i+=1;
}return I;}




/****************************************************************************************************************************/
