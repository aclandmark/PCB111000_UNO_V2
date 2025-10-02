/*****************************************************************************/
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/eeprom.h>
//#include <stdint.h>
//#include <stdio.h>


#include <avr/wdt.h>
#define wdr()  __asm__ __volatile__("wdr")



//************This area is for project subroutines*************************************************************/
void Char_to_PC_Local(char data)
{ while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;}

/********************************************************************************************************/
char waitforkeypress_Local (void){                    //Wait here indefinitely but prevent WDTime-out
while (!(UCSR0A & (1 << RXC0)))wdr(); 
return UDR0;}   



char isCharavailable_Local (int m){int n = 0;
while (!(UCSR0A & (1 << RXC0)))                      //Return 1 immediately that a character is received
{n++; wdr();                                        //No character yet: Increment counter                     
if (n>8000) {m--;n = 0;}if (m == 0)return 0;}        //Counter overflows before a character has been received: return zero
return 1;}


char Char_from_PC_Local(void)                         //Return character detected by "isCharavailable()"
{return UDR0;}


void String_to_PC_Local(const char s[]){                //Transmits a sequence (string) of characters and requires the address in program memory of the first one
int i = 0;                                              //"i" gives the relative address of the next character to be transmitted
while(i < 200){                                         //Transmits up to 200 characters using "Char_to_PC()" or untill the null (\0) character is detected
if(s[i] == '\0') break;
Char_to_PC_Local(s[i++]);}}                             //Transmit character and increment "i" so that it addresses (points to) the next one.



void setup_PC_comms_Basic_Local (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
UCSR0B = 0;
UBRR0H = UBRROH_N;                    
UBRR0L = UBRR0L_N;                  
UCSR0A = (1 << U2X0);
UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);}




/*********************************************************************************************/
