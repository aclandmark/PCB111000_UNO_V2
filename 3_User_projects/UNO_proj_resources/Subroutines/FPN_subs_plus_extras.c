
long FPN_to_Significand(float, long *, char *);
long Fraction_to_Binary_Signed(long, long);
float Assemble_FPN(unsigned long, int);
long unpack_FPN(float, int *, char*);
void Print_long_as_binary(long);
float Scientifc_num_to_FPN(float, char);


char PCMSK0_backup, PCMSK2_backup, float_display_mode;



/*****************************************************************************/
#define pci_on_sw1_and_sw2_enabled (PCMSK2 & 0x84) == 0x84
#define pci_on_sw3_enabled (PCMSK0 & 0x04) == 0x04
#define PCIenabled ((pci_on_sw1_and_sw2_enabled) || (pci_on_sw3_enabled))
#define disable_pci_on_sw1_and_sw2  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define disable_pci_on_sw3  PCMSK0 &= (~(1 << PCINT2));



/*****************************************************************************/
#define I2C_Tx_float_display_control \
{\
PCMSK0_backup= PCMSK0;\
PCMSK2_backup= PCMSK2;\
float_display_mode = '0';\
if (PCIenabled){disable_pci_on_sw3;disable_pci_on_sw1_and_sw2;}\
while(1){\
if(switch_3_down)float_display_mode = '1'; else float_display_mode = '0';\
if((switch_1_down)||(switch_2_down))float_display_mode = '2';\
waiting_for_I2C_master;\
send_byte_with_Nack(float_display_mode);\
clear_I2C_interrupt;\
if(float_display_mode == '2')break;}\
PCMSK0 = PCMSK0_backup;\
PCMSK2 = PCMSK2_backup;}



float Scientific_number_from_KBD(char *sign){

char digits[15];
long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
long FPN_digits;
float FPN;
char sign_local;


Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator, &sign_local);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);                            //0.1011000011.... for example
FPN = Assemble_FPN(FPN_digits, twos_expnt);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);
if (sign_local == '-'){*(long*)&FPN |= (unsigned long) 0x80000000; }
*sign = sign_local;
return FPN;}




/******************************************************************************/
long Fraction_to_Binary_Signed(long rem, long Den){			//Converts rem/Den to 0.1xxxxxxxxxx... where x is either 0 or 1
long Result = 0; 						
unsigned int res_LB=0; 
char sign = '+';

if((rem >= -0x40000000) && (rem < 0x40000000) &&
(Den >= -0x40000000) && (Den < 0x40000000));
else{rem /= 2; Den /= 2;}

if ((rem < 0) || (Den < 0)) sign = '-';
if ((rem < 0) && (Den < 0)) sign = '+';

for(int n = 1; n <= 15; n++){								//bit 0 is reserved for sign bit
if ((2*(rem))/Den)(Result) |= (1 << (15-n));				//Uses 32 integer divisions to obtain the bicimal points
rem = (2*(rem))%Den; }
Result = Result << 16;
for(int n = 0; n <= 15; n++){
if ((2*(rem))/Den)(res_LB) |= (1 << (15-n));
rem = (2*(rem))%Den;}
Result += res_LB;											//Result is not affected by the signs of rem and Den
if(sign == '-')Result = Result * (-1);		//Result = ~Result;							//Inverting the sign bit is equivalent to adding -1
return Result;}



/******************************************************************************/
/*
10 is (10/16) x 2^4 = 0.101 0000 0000 0000 00000 0000 0000 0000 = 0x50000000 x 2^4 in a form suitable for arithmetic
0.1 is (8/10) / 2^3 = 0.110 0110 0110 0110 0110 0110 0110 0110 = 0x66666666 / 2^3 in a form suitable for arithmetic
*/

float Scientifc_num_to_FPN(float FPN, char tens_expnt )
{int twos_expnt, twos_expnt_old;


long FPN_digits;
int FPN_shift;
char sign = '+';


FPN_digits = unpack_FPN(FPN, &twos_expnt, &sign);				//Returns positive number with sign

if (tens_expnt > 0 ){
for(int m = 0; m < tens_expnt; m++){

while (FPN_digits >= 0x66666666)										//Multiply by 10: (Divide by 0.1) Use denominator of 0x50000000 with a tws_exponent of 3
{FPN_digits /= 2; twos_expnt += 1;}

FPN_digits = 
Fraction_to_Binary_Signed(FPN_digits, 0x66666666);

for(int m = 0; m <= 2; m++){
if (twos_expnt < 128)twos_expnt += 1;
else {twos_expnt = 129;FPN_digits = 0;}}}}


if (tens_expnt < 0 ){
for(int m = 0; m < tens_expnt * -1; m++){

while (FPN_digits >= 0x50000000)										//Divide by 10 Use denominator of 0x50000000 with a twos_exponent of 4
{FPN_digits /= 2; twos_expnt += 1; }

FPN_digits = 
Fraction_to_Binary_Signed((long)FPN_digits, 0x50000000 );

for(int m = 0; m <= 3; m++){
if (twos_expnt > -160){twos_expnt -= 1;}}}

if(twos_expnt <= -126){
FPN_shift = twos_expnt * (-1) - 125;
FPN_digits = (unsigned long)FPN_digits >> FPN_shift;
twos_expnt = -126;}}

FPN = Assemble_FPN((unsigned long)FPN_digits, twos_expnt);
if (sign == '-')
*(long*)&FPN = *(long*)&FPN | (unsigned long)0x80000000;				//Set the sign bit
return FPN;}



/*********************************************************************/
void I2C_Tx_float_num(long L_number, char expnt){
char s[5];
char num_bytes=5; char mode='K';
for(int m=0; m<=3; m++){s[m] = (L_number >> (8*(3-m)));}
s[4] = expnt;
I2C_Tx(num_bytes,mode, s);}



/*********************************************************************/
void I2C_FPN_to_display(float num){
  long Denominator;
  long Significand;
  char expnt;

Significand = FPN_to_Significand(num, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;}



/************************************************************************/
float Assemble_FPN(unsigned long FPN_digits, int twos_expnt)
{char sign = '+';

if (FPN_digits & (unsigned long)0x80000000)
{FPN_digits = (long)FPN_digits * (-1); sign = '-';}								//If negative convert to positive and set sign.

FPN_digits = FPN_digits >> 6;													//(Sign bit (bit 31) is zero. MSB (bit 30) is always 1. FPN has 31-6 = 25 bits.
FPN_digits += 1;																//Round LSB. 						
FPN_digits = FPN_digits >> 1;													//Remove rounded bit. FPN now has 25-1 = 24 bits
twos_expnt -= 1;																//Number converted from 0.11010101 (used for arithmetic) to 1.1010101 (Standard FPN format)
twos_expnt += 127;										
FPN_digits = FPN_digits  &  (~((unsigned long)0x80000000 >> 8));				//Clear bit 23 (which is always 1)
FPN_digits = FPN_digits | ((long)twos_expnt << 23);								//Exponent occupies bits 23 to 30 (bit 31 reserved for sign)
if (sign == '-')FPN_digits = FPN_digits | (unsigned long)0x80000000;			//Rienstate sign bit
return *(float*)&FPN_digits;}



/*********************************************************************/
long unpack_FPN(float FPN, int *twos_expnt, char * sign)
{long FPN_digits, FPN_as_long;

FPN_as_long = (*(long*)&FPN);
if( FPN_as_long & (((unsigned long)0x80000000)))
{ FPN_as_long = FPN_as_long & (~((unsigned long)0x80000000));					//If negative remove sign bit
*sign = '-';}else *sign = '+';

*twos_expnt = (FPN_as_long >> 23) - 127;
FPN_digits = (FPN_as_long & 0x7FFFFF);											//Isolate the binary points 23 bits
FPN_digits |= ((unsigned long)0x80000000 >> 8);									//Add in the missing 1 to make 24
FPN_digits = FPN_digits << 7;													//Fill entire long number space, BUT leaving sign bit empty
*twos_expnt += 1;																//Convert from 1.1010101 to 0.11010101

return FPN_digits;}




/*********************************************************************/




