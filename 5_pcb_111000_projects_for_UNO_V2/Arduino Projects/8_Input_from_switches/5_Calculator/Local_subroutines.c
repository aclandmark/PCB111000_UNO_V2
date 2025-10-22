
#include <avr/io.h>

long FPN_to_Significand_Local(float, long *, char *);

#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)


#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);

void I2C_Tx(char, char, char*);
void send_byte_with_Nack(char);
long FPN_to_Significand(float, long *, char *);
char PCMSK0_backup, PCMSK2_backup, float_display_mode;



/*****************************************************************************/


#define disable_pci_on_sw1_and_sw2_Local      PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define disable_pci_on_sw3_Local              PCMSK0 &= (~(1 << PCINT2));  
#define     disable_pci_Local                 disable_pci_on_sw1_and_sw2_Local; disable_pci_on_sw3_Local;

#define pci_on_sw1_and_sw2_enabled (PCMSK2 & 0x84) == 0x84
#define pci_on_sw3_enabled (PCMSK0 & 0x04) == 0x04
#define PCIenabled ((pci_on_sw1_and_sw2_enabled) || (pci_on_sw3_enabled))




/*****************************************************************************/
#define I2C_Tx_float_display_control \
{\
PCMSK0_backup= PCMSK0;\
PCMSK2_backup= PCMSK2;\
float_display_mode = '0';\
if (PCIenabled){disable_pci_Local;}\
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
long Fraction_to_Binary_Signed(long rem, long Den){      //Converts rem/Den to 0.1xxxxxxxxxx... where x is either 0 or 1
long Result = 0;            
unsigned int res_LB=0; 
char sign = '+';

if((rem >= -0x40000000) && (rem < 0x40000000) &&
(Den >= -0x40000000) && (Den < 0x40000000));
else{rem /= 2; Den /= 2;}

if ((rem < 0) || (Den < 0)) sign = '-';
if ((rem < 0) && (Den < 0)) sign = '+';

for(int n = 1; n <= 15; n++){               //bit 0 is reserved for sign bit
if ((2*(rem))/Den)(Result) |= (1 << (15-n));        //Uses 32 integer divisions to obtain the bicimal points
rem = (2*(rem))%Den; }
Result = Result << 16;
for(int n = 0; n <= 15; n++){
if ((2*(rem))/Den)(res_LB) |= (1 << (15-n));
rem = (2*(rem))%Den;}
Result += res_LB;                     //Result is not affected by the signs of rem and Den
if(sign == '-')Result = Result * (-1);            //Inverting the sign bit is equivalent to adding -1
return Result;}


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
  
Significand = FPN_to_Significand_Local(num, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;}



/***********************************************************************************************************************************/
long FPN_to_Significand_Local(float FPN, long * Denom, char * expnt){        //Used for communication with mini-OS
float FPN_bkp;
char sign;

*expnt = 0;
*Denom = 1;

sign = '+';
if (FPN < 0){FPN = FPN * (-1); sign = '-';}

FPN_bkp = FPN;
if(FPN_bkp >= 1){while (FPN >= 1){FPN = FPN/10.0; *expnt += 1;}}
if(FPN_bkp < 0.1){while (FPN < 0.1){FPN = FPN*10.0; *expnt -= 1;}}
while (FPN != (long)FPN) {FPN = FPN * 10.0; *Denom *= 10;}
if (sign == '-')FPN = FPN * (-1);

return (long)FPN; }


/********************************************************************************************************************************/
