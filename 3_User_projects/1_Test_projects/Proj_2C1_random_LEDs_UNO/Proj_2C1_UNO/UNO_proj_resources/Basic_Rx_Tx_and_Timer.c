



#define T0_delay_10ms 	7,178

void Timer_T0_sub(char, unsigned char);



/**********************************************************************************************************************************************************************************/
void setup_PC_comms_Basic (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
UCSR0B = 0;
UBRR0H = UBRROH_N; 														//Set the USART Baud Rate Register
UBRR0L = UBRR0L_N;  
UCSR0A = (1 << U2X0);													//Double speed operation
UCSR0B = (1 << RXEN0) | (1<< TXEN0);									//Enable Receive and transmitter units 
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 								//8 bit asynchronous operation, no parity



/**********************************************************************************************************************************************************************************/
void Timer_T0_10mS_delay_x_m(int m)
{for (int n = 0; n < m; n++)
{Timer_T0_sub(T0_delay_10ms);wdr();}}									//Reset WDTimer every 10 mS



/**********************************************************************************************************************************************************************************/
void Timer_T0_sub(char Counter_speed, unsigned char Start_point){ 		//Operation is same as that of Timer 1
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



