

/*
Basic serial comms functions used to exchange data with the PC
Two versions are given:
	Those ending in _Basic are based on data sheet definitions and provide a useful learning aide
	Those ending in _A use the Arduino library and are probably the sensible choice for most user applications
Note however 
	Arduino uses the UART receive interrupt vector  ISR(USART_RX_vect)
	If the user application requires this then the Arduino library cannot easily be used
	
Timer T1 and T2 functions are also included
Note T0 is used exclusively for one wire comms with the display driver/mini-OS device ATTtiny 1606)
*/






#define T2_delay_10ms 	7,178

#define T1_delay_10ms	2,0xD8EF
#define T1_delay_50ms 5,0xFE78
#define T1_delay_100ms 5,0xFCF2
#define T1_delay_250ms 5,0xF85F
#define T1_delay_500ms 5,0xF0BE

#define T1_delay_1sec 5,0xE17D
#define T1_delay_2sec 5,0xC2FB
#define T1_delay_4sec 5,0x85F7


void setup_PC_comms_Basic (unsigned char, unsigned char);
void Timer_T2_10mS_delay_x_m(int);
void Timer_T2_sub(char, unsigned char);

char isCharavailable_Basic (int);
char isCharavailable_A (int);
char waitforkeypress_Basic(void);
char waitforkeypress_A(void);

char Char_from_PC_Basic(void);
void String_to_PC_Basic(const char*);
void Char_to_PC_Basic(char);

char decimal_digit (char);
char wait_for_return_key_A(void);
void Check_num_for_to_big_or_small(float);



/**********************************************************************************************************************************************************************************/
void setup_PC_comms_Basic (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
UCSR0B = 0;
UBRR0H = UBRROH_N; 														//Set the USART Baud Rate Register
UBRR0L = UBRR0L_N;  
UCSR0A = (1 << U2X0);													//Double speed operation
UCSR0B = (1 << RXEN0) | (1<< TXEN0);									//Enable Receive and transmitter units 
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 								//8 bit asynchronous operation, no parity



/**********************************************************************************************************************************************************************************/
void Timer_T1_sub(char Counter_speed, unsigned int Start_point){ 
TCNT1H = (Start_point >> 8);
TCNT1L = Start_point & 0x00FF;											//TCNT1 counts up from its start_point to 0x10000 (zero)
TIFR1 = 0xFF;															//Clear timer interrupt flags
TCCR1B = Counter_speed;	
while(!(TIFR1 && (1<<TOV1)));											//Wait here for timer to overflow (count from 0xFFFF to zero)
TIFR1 |= (1<<TOV1); 													//Clear overflow flag
TCCR1B = 0;}															//Halt counter



/**********************************************************************************************************************************************************************************/
void Timer_T2_10mS_delay_x_m(int m)
{for (int n = 0; n < m; n++)
{Timer_T2_sub(T2_delay_10ms);wdr();}}									//Reset WDTimer every 10 mS



/**********************************************************************************************************************************************************************************/
void Timer_T2_sub(char Counter_speed, unsigned char Start_point){ 		//Operation is same as that of Timer 1
TCNT2 = Start_point;
TCCR2B = Counter_speed;
while(!(TIFR2 & (1<<TOV2)));
TIFR2 |= (1<<TOV2); TCCR2B = 0;}



/**********************************************************************************************************************************************************************************/
char isCharavailable_Basic (int m){int n = 0;
while (!(UCSR0A & (1 << RXC0)))											//Return 1 immediately that a character is received
{n++; wdr();															//No character yet: Increment counter											
if (n>8000) {m--;n = 0;}if (m == 0)return 0;}							//Counter overflows before a character has been received: return zero
return 1;}



/**********************************************************************************************************************************************************************************/
char waitforkeypress_Basic (void){										//Wait here indefinitely but prevent WDTime-out
while (!(UCSR0A & (1 << RXC0)))wdr();	
return UDR0;}															//Return the character from the USART data register



/**********************************************************************************************************************************************************************************/
char Char_from_PC_Basic(void)											//Return character detected by "isCharavailable()"
{return UDR0;}



/**********************************************************************************************************************************************************************************/
void Char_to_PC_Basic(char data){														
while (!(UCSR0A & (1 << UDRE0)));										//Wait for transmit buffer to be ready to accept new data
UDR0 = data;}															//Load data register with "data" and it will immediately be transmitted



/**********************************************************************************************************************************************************************************/
void String_to_PC_Basic(const char s[]){								//Transmits a sequence (string) of characters and requires the address in program memory of the first one
int i = 0;																//"i" gives the relative address of the next character to be transmitted
while(i < 200){															//Transmits up to 200 characters using "Char_to_PC()" or untill the null (\0) character is detected
if(s[i] == '\0') break;
Char_to_PC_Basic(s[i++]);}}												//Transmit character and increment "i" so that it addresses (points to) the next one.




/**********************************************************************************************************************************************************************************/
char decimal_digit (char data){											//Returns 1 if data is a character of 0 to 9 inclusive
if (((data > '9') || (data < '0')) )return 0;							//Otherwise return zero
else return 1;}



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
void Check_num_for_to_big_or_small(float num)											//Exits if the result of floating point arithmetic exceeds permitted limits 
{unsigned long * long_ptr;
long_ptr = (unsigned long *)&num;														//Enables floating point number to be read as a 32 bit integer 
if (*long_ptr == 0x7F800000){Serial.write("+ve Num too large\r\n");SW_reset;}
if (*long_ptr == 0xFF800000){Serial.write("-ve Num too large\r\n");SW_reset;}
if (*long_ptr == 0X0){Serial.write("+ve Num too small\r\n");SW_reset;}
if (*long_ptr == 0X80000000){Serial.write("-ve Num too small\r\n");SW_reset;}}








