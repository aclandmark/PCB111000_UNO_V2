







void SBtoAL(char*, long, char);
void NumericString_to_PC(char*);
void my_utoa(char, unsigned int,  char*, char);

void newline(void){sendString ("\r\n");}
void sendSpace(void){sendString ("  ");}

	
/*******************************************************************************************/
void Timer_T0_10mS_delay_x_m(int m)
{for (int n = 0; n < m; n++){Timer_T0_sub(T0_delay_10ms);}}


/*******************************************************************************************/
void Timer_T0_sub(char Counter_speed, unsigned char Start_point){ 
TCNT0 = Start_point;
TCCR0B = Counter_speed;
while(!(TIFR0 & (1<<TOV0)));
TIFR0 |= (1<<TOV0); TCCR0B = 0;}




/*******************************************************************************************/
void Timer_T1_sub(char Counter_speed, unsigned int Start_point){ 
TCNT1H = (Start_point >> 8);
TCNT1L = Start_point & 0x00FF;
TIFR1 = 0xFF;
TCCR1B = Counter_speed;
while(!(TIFR1 & (1<<TOV1)));
TIFR1 |= (1<<TOV1); 
TCCR1B = 0;}




/*******************************************************************************************/
char waitforkeypress(void){
while (!(UCSR0A & (1 << RXC0)));
return UDR0;}


/*******************************************************************************************/
char receiveChar(void)
{return UDR0;}


/*******************************************************************************************/
char isCharavailable (int m){int n = 0;		
while (!(UCSR0A & (1 << RXC0))){n++;
if (n>4000) {m--;n = 0;}if (m == 0)return 0;}
return 1;}


/*******************************************************************************************/
void binUnwantedChars (void){char bin_char;
while(1){if (isCharavailable(2)==1)bin_char = receiveChar();else break;}}



/*******************************************************************************************/
void sendChar(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}


/*******************************************************************************************/
void sendString(const char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }

void sendString_V(char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }

void send_NUM_String(char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }

/*******************************************************************************************/
void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
{twenty_msec_delay;}	
UCSR0B = 0;
UBRR0H = UBRROH_N;  // for 9.6 and 38.4kB/s;
UBRR0L = UBRR0L_N;  //25 for 38.4kHz baud rate for 8MHz osc. freq.
UCSR0A = (1 << U2X0);
UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 



/*********************************************************************/
void Num_to_PC(char radix, long int_num){
char array[12];												//long  has 10 chars + sign + null terminator	
SBtoAL(array, int_num, radix);								//calls the Binary to askii subroutine
NumericString_to_PC(array);}	




/*********************************************************************/
void SBtoAL(char array[], long num, char radix){					//Signed Binary to Askii
int m=0;
long sign;

if (num == 0x80000000){
switch(radix){
case 10: array[0] = '8';array[1] = '4'; array[2] = '6';			//0x80000000 * -1 = 0
array[3] = '3';array[4] = '8';array[5] = '4'; array[6] = '7';
array[7] = '4';array[8] = '1';array[9] = '2';
array[10] = '-'; array[11] = '\0'; break;

case 16: array[0] = '0';array[1] = '0'; array[2] = '0';			//0x80000000 * -1 = 0
array[3] = '0';array[4] = '0';array[5] = '0'; array[6] = '0';
array[7] = '8';array[8] = '-';array[9] = '\0';
array[10] = '\0'; array[11] = '\0'; break; } return;}

for (int n = 0; n <=11; n++)array[n] = 0;	
if ((sign = num) < 0) num = num * (-1);

do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);
if (sign < 0) {array[m] = '-';m++;}}





/*********************************************************************/
void NumericString_to_PC(char* s){					
int n=0;
while (s[n] != '\0')n++;									//scroll to end of string counting the number of characters
for(int m = n; m; m--)sendChar(*(s + m-1));}	



void sendHex(char radix, unsigned int Hex){  char print_out_string[6];
my_utoa(radix, Hex, print_out_string, 'Z'); send_NUM_String(print_out_string);sendString(" ");}//include leading zeros


void my_utoa(char radix, unsigned int n,  char s[], char mode){
unsigned char i,sl,j,c;
i=0; sl=0;
do{sl++;
s[i] = n % radix;
if (s[i] < 10) s[i]+= '0';
else s[i] += '7';
i++;
}while((n/=radix)>0);
if ((i < 4) && (mode == 'Z')){while(i < 4) {s[i++] = '0';sl++;}}	//new line
s[i] = '\0';
for(i=0, j=sl-1; i<j; i++,j--){
c=s[i];  s[i] = s[j];  s[j] = c;}}





