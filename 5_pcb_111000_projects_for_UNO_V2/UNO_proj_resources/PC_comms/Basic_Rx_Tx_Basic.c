

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







void Timer_T0_sub(char, unsigned char);
void String_to_PC_Basic(const char*);

void I2C_Tx_8_byte_array(char *);
char wait_for_return_key_Basic(void);
long I2C_displayToNum(void);

void I2C_Tx_initiate_mode(char);
void I2C_Tx(char, char, char*);

void send_byte_with_Ack(char);
void send_byte_with_Nack(char);
unsigned char receive_byte_with_Ack(void);
unsigned char receive_byte_with_Nack(void);


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
void Num_string_to_PC_Basic(char s[]){
{int m = 0;while(1)
{if(!(s[m]))break; 
else Char_to_PC_Basic(s[m++]);}}}



/**********************************************************************************************************************************************************************************/
char decimal_digit_Basic (char data){											//Returns 1 if data is a character of 0 to 9 inclusive
if (((data > '9') || (data < '0')) )return 0;							//Otherwise return zero
else return 1;}



/**********************************************************************************************************************************************************************************/
void Cal_UNO_pcb_A_Basic(void)
{unsigned char OSCCAL_mini_OS;
int error_mag;

User_prompt_Basic;
I2C_Tx_initiate_mode('R');
String_to_PC_Basic("\r\nPCB_A (mini_OS) device calibrating");
waiting_for_I2C_master;  
OSCCAL_mini_OS = receive_byte_with_Ack();
error_mag = receive_byte_with_Ack() << 8;
error_mag += receive_byte_with_Nack();
clear_I2C_interrupt;
String_to_PC_Basic("\r\nOSCCAL user value   "); 
itoa(OSCCAL_mini_OS, num_as_string, 10);String_to_PC_Basic(num_as_string);

String_to_PC_Basic("\r\ncalibration error  "); 
itoa(error_mag, num_as_string, 10);String_to_PC_Basic(num_as_string);
if (error_mag < 750) String_to_PC_Basic("  OK\r\n");}



/***************************************************************************************************************************************************/
void Read_Hello_world_string(void){
char receive_byte;

I2C_Tx_initiate_mode('H');
waiting_for_I2C_master;									//Initiate comuninations:master to send string
do{receive_byte = receive_byte_with_Ack();				//print out string as characters are received
if(receive_byte)Char_to_PC_Basic(receive_byte);}
while(receive_byte);									//Detect '\0' character used to terminate a string
receive_byte_with_Nack();								//Receve a second null char at the end of the string
clear_I2C_interrupt;}									//Complete transaction




/************************************************************************/
void I2C_Rx_get_version(char str_type){
char num_bytes=1; char mode='P';
char s[2];

s[0]= str_type; s[1]=0;
I2C_Tx(num_bytes,mode, s);
waiting_for_I2C_master;
num_bytes = (receive_byte_with_Ack() - '0') * 10;
num_bytes += (receive_byte_with_Ack() - '0');
for (int m = 0; m < num_bytes; m++){
if (m ==(num_bytes-1)){Char_to_PC_Basic(receive_byte_with_Nack());}
else {Char_to_PC_Basic(receive_byte_with_Ack());}}
TWCR = (1 << TWINT);}

