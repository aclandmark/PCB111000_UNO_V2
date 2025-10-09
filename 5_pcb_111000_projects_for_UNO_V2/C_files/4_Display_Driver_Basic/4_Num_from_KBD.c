


//Acquire an integer string from the keyboard and returns the binary equivalent
long Int_KBD_to_display_A_Local(char display_buffer[]){                

char keypress;
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;              

while(1){
if ((keypress = wait_for_return_key_Basic())=='\r')break;  		//Detect return key press
if (!(decimal_digit_Basic(keypress)))continue;

if(display_buffer[0]){for(int n = 8; n>=1; n--)       //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];}
display_buffer[0] = keypress;                         //Add new keypress

I2C_Tx_8_byte_array(display_buffer);}       		//Update display includes "cr_keypress"

I2C_Tx_any_segment_clear_all();
wdr();_delay_ms(50);wdr();_delay_ms(50);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();

return Long_Num_from_mini_OS;}