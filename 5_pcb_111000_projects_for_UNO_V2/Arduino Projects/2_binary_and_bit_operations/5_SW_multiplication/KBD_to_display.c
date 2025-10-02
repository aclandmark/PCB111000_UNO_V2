



#include <util/delay.h>
#include <avr/wdt.h>


char waitforkeypress_Basic (void);
char decimal_digit_Basic (char);
void Char_to_PC_Basic(char);
char wait_for_return_key_Basic(void);
void I2C_Tx_8_byte_array(char *);
long I2C_displayToNum(void);
void I2C_Tx_any_segment_clear_all(void);


#define wdr()  __asm__ __volatile__("wdr")




long Int_KBD_to_display_Local(char display_buffer[]){                
char keypress;
char sign = '+';
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;              

while(1){
keypress = waitforkeypress_Basic();
if ((!(decimal_digit_Basic(keypress)))
&& (keypress != '-'))continue;                                     //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}

if (keypress != '-')Char_to_PC_Basic ('+');
Char_to_PC_Basic(keypress);
I2C_Tx_8_byte_array(display_buffer);

while(1){
if ((keypress = wait_for_return_key_Basic())=='\r')break;          //Detect return key press
if (!(decimal_digit_Basic(keypress)))continue;
Char_to_PC_Basic(keypress);
if(display_buffer[0]){for(int n = 8; n>=1; n--)                   //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];}
display_buffer[0] = keypress;                                    //Add new keypress

I2C_Tx_8_byte_array(display_buffer);}                             //Update display includes "cr_keypress"

I2C_Tx_any_segment_clear_all();
wdr();_delay_ms(50);wdr();_delay_ms(50);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();

return Long_Num_from_mini_OS;}





/**********************************************************************************************************************/
