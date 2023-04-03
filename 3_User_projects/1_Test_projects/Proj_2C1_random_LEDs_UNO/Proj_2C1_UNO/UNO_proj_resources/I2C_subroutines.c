


void send_byte_with_Ack(char byte){
TWDR = byte;											//Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);		//clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));}

/***********************************************************/
void send_byte_with_Nack(char byte){
TWDR = byte;										//Send payload size: Zero in this case
TWCR = (1 << TWINT) | (1 << TWEN);		//clear interrupt and set Enable Acknowledge
while (!(TWCR & (1 << TWINT)));}

/***********************************************************/
unsigned char receive_byte_with_Ack(void){
char byte;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);		//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));						//Wait for interrupt
byte = TWDR;
return byte;}

/***********************************************************/
unsigned char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);		//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));						//Wait for interrupt
byte = TWDR;
return byte;}

/***********************************************************/
void I2C_Tx(char num_bytes, char mode, char s[]){
waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Ack(mode);
for (int m = 0;  m < num_bytes; m++){
if (m==num_bytes-1){send_byte_with_Nack(s[m]);}
else {send_byte_with_Ack(s[m]);}}
TWCR = (1 << TWINT);}										//Clear interrupt and close I2C slave


void I2C_Tx_initiate_mode(char mode){
char num_bytes = 0;
waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT);}


/************************************************************************/
void I2C_Tx_2_integers(unsigned int s1, unsigned int s2){			
char num_bytes=4; char mode = 1; char s[4];
for (int m = 0;  m < 4; m++){
switch (m){
case 0: s[m] = s1; break; 											//Send S1 lower byte
case 1: s[m] = s1 >> 8; break; 									//Send S1 higher byte									
case 2: s[m] = s2; 	break;											//Send S1 lower byte
case 3: s[m] = s2 >> 8; break;}}									//Send S1 higher byte
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_any_segment(char segment, char digit_num){
char s[2]; char num_bytes=2; char mode = 3;
s[0] = segment;
s[1] = digit_num; 
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_any_segment_clear_all(void){
char segment = 'a'; char digit_num = 0;
char s[2]; char num_bytes=2; char mode = 2;
s[0] = segment;
s[1] = digit_num; 
I2C_Tx(num_bytes,mode, s);}

/************************************************************************/
void I2C_Tx_LED_dimmer_UNO(void){
char Dimmer_control = 0; 
unsigned int n = 0, Port_1=0xCCCC;

while((PINB & 0x04)^0x04) 							//while switch_3 down
{n++; if (n >= 60000)break;}


while((PINB & 0x04)^0x04)
{Dimmer_control = (Dimmer_control + 1)%4;
if(!(Dimmer_control))continue;
I2C_Tx(1, 'Q', &Dimmer_control);
I2C_Tx_2_integers(Port_1, ~Port_1);
Timer_T0_10mS_delay_x_m(50);}}


/************************************************************************/
void Cal_UNO_pcb_A(void)
{unsigned char OSCCAL_mini_OS;
char num_as_string[12];
int error_mag;

User_prompt;
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



/**********************************************************************************/