
#include <util/delay.h>
#include <avr/wdt.h>
#define wdr()  __asm__ __volatile__("wdr")


char wait_for_return_key_Basic(void);
char decimal_digit_Basic (char);
void I2C_Tx_any_segment_clear_all (void);
void I2C_Tx_8_byte_array (char*);


char Int_KBD_to_display_A_Local(char display_buffer[]){                //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;
char keypress_counter = 0;

for(int n = 0; n< 16; n++) display_buffer[n] = 0;              

while(1){
if ((keypress = wait_for_return_key_Basic())=='\r')break;         //Detect return key press (i.e \r or\r\n)
if (!(decimal_digit_Basic(keypress)))continue;

if(display_buffer[0]){for(int n = 15; n>=1; n--)                                      //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];}
display_buffer[0] = keypress;                                  //Add new keypress           

I2C_Tx_8_byte_array(display_buffer);                           //Update display includes "cr_keypress"                                                 
keypress_counter += 1;

if (display_buffer[14])break;}

display_buffer[keypress_counter] = 0;

I2C_Tx_any_segment_clear_all();
wdr();_delay_ms(50);wdr();_delay_ms(50);
I2C_Tx_8_byte_array(display_buffer);

return keypress_counter;}
