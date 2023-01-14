
long FPN_to_Significand(float, long *, char *);
long Fraction_to_Binary_Signed(long, long);
long Assemble_FPN(long, unsigned char);
long unpack_FPN(long, unsigned char *, char);

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
float Scientifc_num_to_FPN(long FPN_as_Long, char tens_expnt )
{unsigned char twos_expnt;
long FPN_part;

FPN_part = unpack_FPN(FPN_as_Long, &twos_expnt, tens_expnt);

if (tens_expnt > 0 ){
for(int m = 0; m < tens_expnt; m++){									//Fails for 10,100,1000 etc

while (FPN_part >= 0x66666666)								//Tens denominator is 0x50000000 with a tws_exponent of 3
{FPN_part /= 2; twos_expnt += 1;}

FPN_part = 
Fraction_to_Binary_Signed(FPN_part, 0x66666666);
twos_expnt += 3;}}


if (tens_expnt < 0 ){
for(int m = 0; m < tens_expnt * -1; m++){									//Fails for 10,100,1000 etc

while (FPN_part >= 0x50000000)								//Tens denominator is 0x50000000 with a tws_exponent of 3
{FPN_part /= 2; twos_expnt += 1;}

FPN_part = 
Fraction_to_Binary_Signed(FPN_part, 0x50000000);
twos_expnt -= 4;}}


FPN_as_Long = Assemble_FPN(FPN_part, twos_expnt);
return *(float*)&FPN_as_Long;}



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
long Assemble_FPN(long FPN, unsigned char twos_expnt)
{
FPN = FPN >> 7; 
FPN += 1;
FPN = FPN >> 1;
FPN = FPN << 1; 
twos_expnt -= 1;
twos_expnt += 127;
FPN = FPN  &  (~((unsigned long)0x80000000 >> 8));
FPN = FPN | ((long)twos_expnt << 23);
return FPN;}



/*********************************************************************/
long unpack_FPN(long FPN, unsigned char *twos_expnt, char tens_expnt)
{long FPN_part;
*twos_expnt = (FPN >> 23) - 127;
Serial.print ((int)*twos_expnt);
FPN_part = (FPN & 0x7FFFFF);								//Isolate the binary points

FPN_part |= ((unsigned long)0x80000000 >> 8);				//Add in the missing 1
FPN_part = FPN_part << 7;									//Fill entire long number space, leaving sign bit empty

*twos_expnt += 1;

return FPN_part;}



/*********************************************************************/




