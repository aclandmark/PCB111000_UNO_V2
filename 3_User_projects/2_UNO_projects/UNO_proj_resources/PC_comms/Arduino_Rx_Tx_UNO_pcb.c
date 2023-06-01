

/*
Exchanging numbers with a PC: Arduino Serial functions are provided to support these operations.
Basic binary to askii and askii to binary functions explored in "Projects\2_Receiver_Transmitter_Basic" 
are also replaced by standard C library functions such as atoi (askii to integer) and itoa (integer to askii).

Note however 
	Arduino uses the UART receive interrupt vector  ISR(USART_RX_vect)
	If the user application requires this then the Arduino library cannot easily be used
*/




void Check_num_for_to_big_or_small(float);						//Prototype required by Sc_Num_to_PC_A()



/******************************************************************************************/
unsigned long Unsigned_Int_from_PC_A
	(char * num_as_string,char next_char)						//Location for numerical string entered at the PC keyboard 
{char strln;													//Holds the length of the numerical string 
int num;

Serial.flush();   												//Clear the serial buffer
strln = Serial.readBytesUntil('\r',num_as_string, 20);			//Fill serial buffer with keypresses untill -cr- is pressed 
num_as_string[strln] = 0;										//Terminate the numerical string with the null character '\0'

if(next_char){
Serial.write(num_as_string);									//Echo the numerical string to the PC
Serial.write(next_char);}

num = atoi(num_as_string);										//Convert the numerical string to an integer number (atoi)
return (unsigned int )num;}



/******************************************************************************************/
long Int_Num_from_PC_A(char * num_as_string, char bufferlen)
{int strln;
int trailing_bs_counter = 0;

Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, bufferlen);

_delay_ms(1);					///?

//Remove trailing delete chars******************************************************************************************
for(int m = strln; m; m--){if(num_as_string[m-1] == '\b')trailing_bs_counter += 1;else break;}
for(int m = 0; m < (trailing_bs_counter * 2); m++){if(strln == m)break; else num_as_string[strln - m-1] = '\0'; }

//Remove leading delete chars******************************************************************************************
for(int m = 0; m < strln; m++){
   while(num_as_string[0] == '\b')
  {for(int p = 0; p < strln-1; p++){num_as_string[p] = num_as_string[p+1];num_as_string[p+1] = 0;m = 0;}}
 
//Remove remainingdel chars*********************************************************************************************
 if(num_as_string[m] != '\b');
 //else for(int p = m; p < strln-1; p++){num_as_string[p-1] = num_as_string[p+1]; num_as_string[p+1] = '\0';m = 0;} }
else for(int p = m; p < strln-1; p++){num_as_string[p-1] = num_as_string[p+1]; num_as_string[p+1] = 0;num_as_string[p] = 0;m = 0;} } 

num_as_string[strln] = 0;
if(atol(num_as_string) > 0x7FFFF)
{Serial.write("\r\nNumber is too large\r\n"); SW_reset;}
return atol(num_as_string);}



/******************************************************************************************/
float Sc_Num_from_PC_A
(char * num_as_string, int bufferlen )	
{char strln;
int trailing_bs_counter = 0;

Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, bufferlen);  

_delay_ms(1);					///?


//Remove trailing delete chars******************************************************************************************
for(int m = strln; m; m--){if(num_as_string[m-1] == '\b')trailing_bs_counter += 1;else break;}
for(int m = 0; m < (trailing_bs_counter * 2); m++){if(strln == m)break; else num_as_string[strln - m-1] = '\0'; }

//Remove leading delete chars******************************************************************************************
for(int m = 0; m < strln; m++){
   while(num_as_string[0] == '\b')
  {for(int p = 0; p < strln-1; p++){num_as_string[p] = num_as_string[p+1];num_as_string[p+1] = 0;m = 0;}}
 
//Remove remaining del chars*********************************************************************************************
 if(num_as_string[m] != '\b');
  //else for(int p = m; p < strln-1; p++){num_as_string[p-1] = num_as_string[p+1]; num_as_string[p+1] = '\0';m = 0;} }
else for(int p = m; p < strln-1; p++){num_as_string[p-1] = num_as_string[p+1]; num_as_string[p+1] = 0;num_as_string[p] = 0;m = 0;} } 

num_as_string[strln] = 0;
return atof(num_as_string);}												//Askii to float





/******************************************************************************************/
void Unsigned_Int_to_PC_A
(unsigned long Int_num, char * num_as_string, char next_char)
{
ultoa(Int_num, num_as_string, 10);								//Unsigned long to askii							
Serial.print(num_as_string);
Serial.print(next_char);}



/******************************************************************************************/
void Int_Num_to_PC_A
(long Int_num, char * num_as_string, char next_char)			//Same as Unsigned_Int_to_PC()
{
ltoa(Int_num, num_as_string, 10);								//Long to askii
Serial.print(num_as_string);Serial.print(next_char);}



/*****************************************************************************************/
void Sc_Num_to_PC_A
(float num, char pre_dp, char post_dp, char next_char)

{int A = 1;
char sign = '+';
char expt = 0;

Check_num_for_to_big_or_small(num);								//Exit before attempting to do arithmetic with these numbers

if (num < 0){sign = '-'; num = num * (-1);}						//Only process positive numbers

/*while(--pre_dp){A = A*10;}										//Convert FPN to Scientific format (real + exponent)
while (num >= A){num = num/10.0; expt += 1;}
while (num <= A){num = num*10.0; expt -= 1;}*/
if(pre_dp){
while(pre_dp--)A = A*10;}
if (num >= 1.0)while (num >= A){num = num/10.0; expt += 1;}
else {while (num*10.0 < A){num = num*10.0; expt -= 1;}}


if(sign == '-')num = num * (-1);								//Restore sign

Serial.print(num, post_dp);										//Print the real number followed by the exponent
if(expt) {Serial.write ('E'); Serial.print((int)expt);}
Serial.write(next_char);}



/*****************************************************************************************/
float Significand_to_FPN(float num, long denom, char expt){					//Used for communication with mini-OS

char exp_bkp;

exp_bkp = expt;
num = num/(float)denom;

if (exp_bkp > 0)
{while (expt > 0){num = num * 10.0; expt -=1; }}

if (exp_bkp < 0)
{while (expt < 0){num = num / 10.0; expt +=1; }}
return num;}



/*****************************************************************************************/
long FPN_to_Significand(float FPN, long * Denom, char * expnt){				//Used for communication with mini-OS
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




