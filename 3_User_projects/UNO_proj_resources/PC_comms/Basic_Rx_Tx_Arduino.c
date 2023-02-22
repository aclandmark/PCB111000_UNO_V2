


/**********************************************************************************************************************************************************************************/
char isCharavailable_A (int m){int n = 0;								//Version of isCharavailable_Basic() that uses the Arduino library
while (!(Serial.available())){n++;	wdr();			
if (n>8000) {m--;n = 0;}if (m == 0)return 0;}	
return 1;}	



/**********************************************************************************************************************************************************************************/
char waitforkeypress_A (void){											//Version of waitforkeypress_Basic() that uses the Arduino library
while (!(Serial.available()))wdr();	
return Serial.read(); }



/**********************************************************************************************************************************************************************************/
char wait_for_return_key_A(void){                  						//Returns key presses one at a time
char keypress,temp;
while(1){																//Remain in while loop until a character is received
if (isCharavailable_A(8)){												//Pauses but returns 1 immediately that a character is received
keypress = Serial.read();												//Skip if no character has been received 
break;}}																//Exit while loop when character has been read
if((keypress == '\r') || (keypress == '\n')){							//Detect \r\n, \r or \n and converts to \r
if (isCharavailable_A(1)){temp = Serial.read();}
keypress = '\r';}
return keypress;}


/**********************************************************************************************************************************************************************************/
char decimal_digit_A (char data){											//Returns 1 if data is a character of 0 to 9 inclusive
if (((data > '9') || (data < '0')) )return 0;							//Otherwise return zero
else return 1;}




/**********************************************************************************************************************************************************************************/
void Check_num_for_to_big_or_small(float num)											//Exits if the result of floating point arithmetic exceeds permitted limits 
{unsigned long * long_ptr;
long_ptr = (unsigned long *)&num;														//Enables floating point number to be read as a 32 bit integer 
if (*long_ptr == 0x7F800000){Serial.write("+ve Num too large\r\n");SW_reset;}
if (*long_ptr == 0xFF800000){Serial.write("-ve Num too large\r\n");SW_reset;}
if (*long_ptr == 0X0){Serial.write("+ve Num too small\r\n");SW_reset;}
if (*long_ptr == 0X80000000){Serial.write("-ve Num too small\r\n");SW_reset;}}



/**********************************************************************************************************************************************************************************/
void Cal_UNO_pcb_A_Arduino(void)
{unsigned char OSCCAL_mini_OS;
int error_mag;

User_prompt_A;
I2C_Tx_initiate_mode('R');
Serial.write("\r\nPCB_A (mini_OS) device calibrating");
waiting_for_I2C_master;  
OSCCAL_mini_OS = receive_byte_with_Ack();
error_mag = receive_byte_with_Ack() << 8;
error_mag += receive_byte_with_Nack();
clear_I2C_interrupt;
Serial.write("\r\nOSCCAL user value   "); 
itoa(OSCCAL_mini_OS, num_as_string, 10);Serial.write(num_as_string);

Serial.write("\r\ncalibration error  "); 
itoa(error_mag, num_as_string, 10);Serial.write(num_as_string);
if (error_mag < 750) Serial.write("  OK\r\n");}




