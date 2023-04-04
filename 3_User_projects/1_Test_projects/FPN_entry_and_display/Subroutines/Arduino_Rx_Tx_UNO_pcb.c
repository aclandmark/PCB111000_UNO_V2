


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

User_prompt;
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


float Significand_to_FPN(float num, long denom, char expt){

char exp_bkp;

exp_bkp = expt;
num = num/(float)denom;

if (exp_bkp > 0)
{while (expt > 0){num = num * 10.0; expt -=1; }}

if (exp_bkp < 0)
{while (expt < 0){num = num / 10.0; expt +=1; }}
return num;}


/*****************************************************************************************/
long FPN_to_Significand(float FPN, long * Denom, char * expnt){
float FPN_bkp;
char sign;

*expnt = 0;
*Denom = 1;

sign = '+';
if (FPN < 0){FPN = FPN * (-1); sign = '-';}

FPN_bkp = FPN;
//if(FPN_bkp > 1){while (FPN > 1){FPN = FPN/10.0; *expnt += 1;}}
if(FPN_bkp >= 1){while (FPN >= 1){FPN = FPN/10.0; *expnt += 1;}}
if(FPN_bkp < 0.1){while (FPN < 0.1){FPN = FPN*10.0; *expnt -= 1;}}
while (FPN != (long)FPN) {FPN = FPN * 10.0; *Denom *= 10;}
if (sign == '-')FPN = FPN * (-1);
return (long)FPN; }



/*****************************************************************************************/
void Sc_Num_to_PC
(float num, char pre_dp, char post_dp, char next_char)

{int A = 1;
char sign = '+';
char expt = 0;

Check_num_for_to_big_or_small(num);								//Exit before attempting to do arithmetic with these numbers

if (num < 0){sign = '-'; num = num * (-1);}						//Only process positive numbers

while(--pre_dp){A = A*10;}										//Convert FPN to Scientific format (real + exponent)
while (num >= A){num = num/10.0; expt += 1;}
while (num <= A){num = num*10.0; expt -= 1;}

if(sign == '-')num = num * (-1);								//Restore sign

Serial.print(num, post_dp);										//Print the real number followed by the exponent
if(expt) {Serial.write ('E'); Serial.print((int)expt);}
Serial.write(next_char);}


long FPN_to_FPN_digits(float FPN,  long *Denominator){}











