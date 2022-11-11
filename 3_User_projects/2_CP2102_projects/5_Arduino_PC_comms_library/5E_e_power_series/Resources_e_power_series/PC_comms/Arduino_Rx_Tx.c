

/*
Exchanging numbers with a PC: Arduino Serial functions are provided to support these operations.
Basic binary to askii and askii to binary functions explored in "Projects\2_Receiver_Transmitter_Basic" 
are also replaced by standard C library functions such as atoi (askii to integer) and itoa (integer to askii).

Note however 
	Arduino uses the UART receive interrupt vector  ISR(USART_RX_vect)
	If the user application requires this then the Arduino library cannot easily be used
*/




void Check_num_for_to_big_or_small(float);						//Prototype required by Sc_Num_to_PC()




/******************************************************************************************/
unsigned long Unsigned_Int_from_PC
	(char * num_as_string,char next_char)						//Location for numerical string entered at the PC keyboard 
{char strln;													//Holds the length of the numerical string 
int num;

pause_WDT;
Serial.flush();   												//Clear the serial buffer
strln = Serial.readBytesUntil('\r',num_as_string, 20);			//Fill serial buffer with keypresses untill -cr- is pressed 
resume_WDT;
num_as_string[strln] = 0;										//Terminate the numerical string with the null character '\0'

if(next_char){
Serial.write(num_as_string);									//Echo the numerical string to the PC
Serial.write(next_char);}

num = atoi(num_as_string);										//Convert the numerical string to an integer number (atoi)
return (unsigned int )num;}



/******************************************************************************************/
long Int_Num_from_PC(char * num_as_string,char next_char)		//Same as Unsigned_Int_from_PC()
{char strln;

pause_WDT;
Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, 20);
resume_WDT;
num_as_string[strln] = 0;
Serial.write(num_as_string);
Serial.write(next_char);
return atol(num_as_string);}






/******************************************************************************************/
void Unsigned_Int_to_PC
(unsigned long Int_num, char * num_as_string, char next_char)
{
ultoa(Int_num, num_as_string, 10);								//Unsigned long to askii							
Serial.print(num_as_string);
Serial.print(next_char);
}



/******************************************************************************************/
void Int_Num_to_PC
(long Int_num, char * num_as_string, char next_char)			//Same as Unsigned_Int_to_PC()
{
ltoa(Int_num, num_as_string, 10);								//Long to askii
Serial.print(num_as_string);Serial.print(next_char);
}






/*****************************************************************************************/
void Sc_Num_to_PC
(float num, char pre_dp, char post_dp, char next_char)
{int A = 1;
//char keypresses[12];
char sign = '+';
int Exp = 0;

Check_num_for_to_big_or_small(num);								//Exit before attempting to do arithmetic with these numbers

if (num < 0){sign = '-'; num = num * (-1);}						//Only process positive numbers

while(--pre_dp){A = A*10;}										//Convert FPN to Scientific format (real + exponent)
while (num >= A){num = num/10.0; Exp += 1;}
while (num <= A){num = num*10.0; Exp -= 1;}

if(sign == '-')num = num * (-1);								//Restore sign

Serial.print(num, post_dp);										//Print the real number followed by the exponent
if(Exp) {Serial.write ('E'); Serial.print(Exp);}
Serial.write(next_char);}




/******************************************************************************************/
float Sc_Num_from_PC
(char * num_as_string,char next_char)							//Same as Int_Num_from_PC()
{char strln;

pause_WDT;
Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, 20);
resume_WDT;
num_as_string[strln] = 0;
Serial.write(num_as_string);
Serial.write(next_char);
return atof(num_as_string);}									//Askii to float











