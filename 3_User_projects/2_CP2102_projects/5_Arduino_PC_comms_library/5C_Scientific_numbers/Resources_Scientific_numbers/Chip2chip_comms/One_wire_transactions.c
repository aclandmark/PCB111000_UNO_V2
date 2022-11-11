

/*
Subroutines that provide firm ware one wire comms with the mini-OS.
The mini-OS uses similar FW routines to initiate and control the communication.
*/





/*************************************************************************************************************/
void UART_Tx_1_wire(void){
Tx_complete = 0;
One_wire_mode = 1;									//Tx mode
  
PCMSK0 |= 1 << PCINT4;				       			//set up Interrupt on pin change (IPC)
while(!(Tx_complete));}								//Wait here until transmission is complete 



/*************************************************************************************************************/
void UART_Rx_1_wire(void){
One_wire_Rx_char = 0;
Rx_complete = 0;
One_wire_mode = 2;									//Rx mode
   
PCMSK0 |= 1 << PCINT4;		       					//set up IPC to detect start pulse
while (!(Rx_complete));}



/*************************************************************************************************************/
ISR(PCINT0_vect){                                   //Detect low on PINC4

if(PINB4_down){										//If start bit:

PCMSK0 &= (~(1 << PCINT4));                         //clear IPC

if(One_wire_mode == 1){								//Transmit character
Start_clock;
wait_for_half_comms_tick;							//Used to set the baud rate

PORTB |= (1 << PORTB4);                               //WPU

wait_for_comms_tick;

DDRB |= (1 << DDB4);                                 //OH
wait_for_comms_tick;
 for(int m = 0; m <= 7; m++){                        //Eight data bits
    if (One_wire_Tx_char & (1 << (7-m)))
    PORTB |= (1 << PORTB4);                          //OH
    else PORTB &= ~(1 << PORTB4);                    //OL
   wait_for_comms_tick;}

DDRB &= ~(1 << PINB4);
PORTB &= ~(1 << PORTB4);                             //Restore Tri
Tx_complete = 1;
TCCR0B = 0;}

if(One_wire_mode == 2){								//Receive character
Start_clock;
wait_for_half_comms_tick;

for (int m = 0; m <= 7;m++){
  wait_for_comms_tick;
  
One_wire_Rx_char = One_wire_Rx_char << 1;			//Assemble bits as they are received
if (PINB & (1 << PINB4))
{One_wire_Rx_char |= 1;}}
TCCR0B = 0;
Rx_complete = 1;}}}

	
	
	
	
	
/*************************************************************************************************************/
ISR(PCINT1_vect){ 											//Interrogates reset control switch presses
if(PINC5_down){
_delay_ms(25); wdr();
if(PINC5_up)return;
for(int m = 0; m < 5; m++){_delay_ms(45); wdr();}			//Switch bounce delay
if(PINC5_up)
{set_Run_BL_bit; }
else{
LED_1_on; LED_2_on;
for(int m = 0; m < 5; m++){_delay_ms(45); wdr();}			//_delay_ms(250);
if(PINC5_down)												//Wait for switch to be released
{sei();One_wire_Tx_char = 'G'; UART_Tx_1_wire(); }}
SW_reset;}}





