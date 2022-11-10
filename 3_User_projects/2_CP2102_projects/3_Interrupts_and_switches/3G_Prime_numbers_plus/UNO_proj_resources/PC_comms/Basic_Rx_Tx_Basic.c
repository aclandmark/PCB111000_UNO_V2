

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






#define T0_delay_10ms 	7,178

#define T1_delay_10ms	2,0xD8EF
#define T1_delay_50ms 5,0xFE78
#define T1_delay_100ms 5,0xFCF2
#define T1_delay_250ms 5,0xF85F
#define T1_delay_500ms 5,0xF0BE

#define T1_delay_1sec 5,0xE17D
#define T1_delay_2sec 5,0xC2FB
#define T1_delay_4sec 5,0x85F7


void setup_PC_comms_Basic (unsigned char, unsigned char);
void Timer_T0_10mS_delay_x_m(int);
void Timer_T0_sub(char, unsigned char);

char isCharavailable_Basic (int);
char isCharavailable_A (int);
char waitforkeypress_Basic(void);
char waitforkeypress_A(void);

char Char_from_PC_Basic(void);
void String_to_PC_Basic(const char*);
void Char_to_PC_Basic(char);
void newline_Basic(void);

char decimal_digit (char);
void Int_to_PC_Basic (int);
char wait_for_return_key_A(void);
void Check_num_for_to_big_or_small(float);

void I2C_Tx_8_byte_array(char *);
char wait_for_return_key_Basic(void);
long I2C_displayToNum(void);


/**********************************************************************************************************************************************************************************/
void setup_PC_comms_Basic (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
UCSR0B = 0;
UBRR0H = UBRROH_N; 														//Set the USART Baud Rate Register
UBRR0L = UBRR0L_N;  
UCSR0A = (1 << U2X0);													//Double speed operation
UCSR0B = (1 << RXEN0) | (1<< TXEN0);									//Enable Receive and transmitter units 
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 								//8 bit asynchronous operation, no parity



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
void newline_Basic(void){String_to_PC_Basic ("\r\n");}



/**********************************************************************************************************************************************************************************/
void String_to_PC_Basic(const char s[]){								//Transmits a sequence (string) of characters and requires the address in program memory of the first one
int i = 0;																//"i" gives the relative address of the next character to be transmitted
while(i < 200){															//Transmits up to 200 characters using "Char_to_PC()" or untill the null (\0) character is detected
if(s[i] == '\0') break;
Char_to_PC_Basic(s[i++]);}}												//Transmit character and increment "i" so that it addresses (points to) the next one.



/**********************************************************************************************************************************************************************************/
char decimal_digit_Basic (char data){											//Returns 1 if data is a character of 0 to 9 inclusive
if (((data > '9') || (data < '0')) )return 0;							//Otherwise return zero
else return 1;}



/**********************************************************************************************************************************************************************************/
void Int_to_PC_Basic (int number)
{ int i = 0;
  char s[12];
   do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}



/**********************************************************************************************************************************************************************************/
int Int_from_PC_Basic(char digits[]){
char keypress;
for(int n = 0; n<=7; n++) digits[n] = 0; 

do
{keypress =  waitforkeypress_Basic();} 
while (!(decimal_digit_Basic(keypress)));                                      //(non_decimal_char(keypress));  //Not -,0,1,2,3,4,5,6,7,8 or 9
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);

while(1){
if ((keypress = wait_for_return_key_Basic())  =='\r')break;
if (decimal_digit_Basic (keypress))                                           //012345678or9  :Builds up the number one keypress at a time
{for(int n = 7; n>=1; n--)
digits[n] = digits[n-1];                                                //Shifts display left for each keypress
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);}}
            
return I2C_displayToNum();}



/**********************************************************************************************************************************************************************************/
char wait_for_return_key_Basic(void){                  						//Returns key presses one at a time
char keypress,temp;
while(1){																//Remain in while loop until a character is received
if (isCharavailable_Basic(8)){												//Pauses but returns 1 immediately that a character is received
keypress = Char_from_PC_Basic();												//Skip if no character has been received 
break;}}																//Exit while loop when character has been read
if((keypress == '\r') || (keypress == '\n')){							//Detect \r\n, \r or \n and converts to \r
if (isCharavailable_Basic(1)){temp = Char_from_PC_Basic();}
keypress = '\r';}
return keypress;}

