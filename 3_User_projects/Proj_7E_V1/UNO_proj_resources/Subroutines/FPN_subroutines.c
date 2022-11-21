

/******************************************************************************/
long Fraction_to_Binary_Signed(long rem, long Den){
long Result = 0; 						
unsigned int res_LB=0; 
char sign = '+';

if((rem >= -0x40000000) && (rem < 0x40000000) &&
(Den >= -0x40000000) && (Den < 0x40000000));
else{rem /= 2; Den /= 2;}

if ((rem < 0) || (Den < 0)) sign = '-';
if ((rem < 0) && (Den < 0)) sign = '+';

for(int n = 1; n <= 15; n++){					//bit 0 is reserved for sign bit
if ((2*(rem))/Den)(Result) |= (1 << (15-n));
rem = (2*(rem))%Den; }
Result = Result << 16;
for(int n = 0; n <= 15; n++){
if ((2*(rem))/Den)(res_LB) |= (1 << (15-n));
rem = (2*(rem))%Den;}
Result += res_LB;								//Result is not affected by the signs of rem and Den
if(sign == '-')Result = ~Result;				//Inverting the sign bit is equivalent to adding -1
return Result;}


/******************************************************************************/

/*long Binary_points_to_Decimal_Signed (long RHSofBP)
{long  RHSofDP=0;
unsigned int RHSofBP_LB;
RHSofBP_LB = RHSofBP;

for(int n = 0; n <= 15; n++){switch(n){						//bit zero is sign bit
case 0: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP -= 1000000000; break;
case 1: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 500000000; break;
case 2: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 250000000; break;
case 3: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 125000000; break;
case 4: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 62500000; break;
case 5: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 31250000; break;
case 6: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 15625000; break;
case 7: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 7812500; break;
case 8: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 3906250; break;
case 9: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 1953125; break;
case 10: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 976563; break;
case 11: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 488281; break;
case 12: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 244141; break;
case 13: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 122070; break;
case 14: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 61035; break;
case 15: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 30518; break;}}
for(int n = 0; n <= 15; n++){switch (n){
case 0: if (RHSofBP_LB & (1 << (15-n)))RHSofDP +=  15259; break;
case 1: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 7629; break;
case 2: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 3815; break;
case 3: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 1907; break;
case 4: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 954; break;
case 5: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 477; break;
case 6: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 238; break;
case 7: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 119; break;
case 8: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 60; break;
case 9: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 30; break;
case 10: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 15; break;
case 11: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 7; break;
case 12: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 4; break;
case 13: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 2; break;
case 14: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 1; break;
case 15: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 0; break;}}	
return RHSofDP;}*/


/******************************************************************************/
/*char decimalOverflow(char radix, unsigned long Hex, int No_dps){
long inc=1;

Hex += 1000000000;				//define location decimal point
for(int k = 0; k < (8-No_dps); k++)inc = inc*10;
inc = inc*5;
Hex += inc;
if(Hex >= 2000000000)return 0; else return 1;}*/


/******************************************************************************/

/*void Decimal_to_PC(char radix, unsigned long Hex, int No_dps){ char print_out_string[12];
long inc = 1;
char space_counter = 0;

if(Hex == 0) {Serial.write(".0");return;}
Hex += 1000000000;				//define location decimal point

for(int k = 0; k < (8-No_dps); k++)inc = inc*10;
inc = inc*5;
Hex += inc;

SBtoAL(print_out_string, Hex, radix);
for(int k = 0; k <= (8-No_dps); k++)print_out_string[k] = ' ';
print_out_string[9] = '.'; 

{int m = 0;while((print_out_string[m] == '0')||(print_out_string[m] == ' '))
{print_out_string[m] = ' ';m++; space_counter++;}}
NumericString_to_PC(print_out_string);
for(int m = 0; m<space_counter; m++)Serial.write('\b');}*/



/******************************************************************************/
/*void Exp_to_PC (signed char expt){
if (expt < 0) { Serial.write (" E-");Num_to_PC(10,expt*(-1));}
else {Serial.write(" E+");Num_to_PC(10,expt);}}
*/

/*********************************************************************/

void I2C_Tx_float_num(long L_number, char expnt){
char s[5];
char num_bytes=5; char mode='K';
for(int m=0; m<=3; m++){s[m] = (L_number >> (8*(3-m)));}
s[4] = expnt;
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
/*********************************************************************/
/*void Num_to_PC(char radix, long long_num){
char array[12];							//Long has 10 chars + sign + null terminator	
SBtoAL(array, long_num, radix);			//calls the Binary to askii subroutine
NumericString_to_PC(array);}				//Prints characters in reverse order
*/
/*********************************************************************/
/*void SBtoAL(char array[], long num, char radix){					//Signed Binary to Askii
int m=0;
long sign;

if (num == 0x80000000){
switch(radix){
case 10: array[0] = '8';array[1] = '4'; array[2] = '6';		//0x80000000 * -1 = 0
array[3] = '3';array[4] = '8';array[5] = '4'; array[6] = '7';
array[7] = '4';array[8] = '1';array[9] = '2';
array[10] = '-'; array[11] = '\0'; break;

case 16: array[0] = '0';array[1] = '0'; array[2] = '0';		//0x80000000 * -1 = 0
array[3] = '0';array[4] = '0';array[5] = '0'; array[6] = '0';
array[7] = '8';array[8] = '-';array[9] = '\0';
array[10] = '\0'; array[11] = '\0'; break; } return;}

for (int n = 0; n <=11; n++)array[n] = 0;	
if ((sign = num) < 0) num = num * (-1);

do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);
if (sign < 0) {array[m] = '-';m++;}}*/


/*********************************************************************/
/*void NumericString_to_PC(char* s){					
int n=0;
while (s[n] != '\0')n++;							//scroll to end of string counting the number of characters
for(int m = n; m; m--)Serial.write(*(s + m-1));}		//print last character first
*/







