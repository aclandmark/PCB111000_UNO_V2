

/*
Code segments required by the subroutines in "One_wire_transactions.c" and else where
*/



void UART_Tx_1_wire(void);
void UART_Rx_1_wire(void);

volatile char One_wire_Rx_char, One_wire_Tx_char;
volatile char Tx_complete, Rx_complete;

volatile char One_wire_mode;

int Comms_clock;




/**********************************************************************************/
#define  comms_cal \
if (((signed char)eeprom_read_byte((uint8_t*)0x3F6) > -50)\
&&  ((signed char)eeprom_read_byte((uint8_t*)0x3F6) < 50) && ((signed char)eeprom_read_byte((uint8_t*)0x3F6)\
== (signed char)eeprom_read_byte((uint8_t*)0x3F7)))\
{Comms_clock = 200 + (signed char) eeprom_read_byte((uint8_t*)0x3F6);}

//A calibration byte for the one wire comms clock is stored in EEPROM locations 0x3F6/7
//For zero calibration the clock which runs at 1MHz generates a baud rate of 200uS




/************************************************************************************************************************************/
#define setup_one_wire_comms \
PCICR |= (1 << PCIE0); PCMSK0 |= (1 << PCINT4);\
PORTB &= (~(1 << PORTB4));

//One wire comms uses port PB4.  This sets it up as input with interrupt on pin change (IPC)



#define clear_display 								One_wire_Tx_char = 'c';  UART_Tx_1_wire();
#define Reset_ATtiny1606 							One_wire_Tx_char = 'F'; UART_Tx_1_wire();

#define set_up_pin_change_interrupt_on_PC5  		PCICR |= (1 << PCIE1); PCMSK1 |= (1 << PCINT13); 
#define pause_pin_change_interrupt_on_PC5  			PCICR &= (~(1 << PCIE1)); 
#define reinstate_pin_change_interrupt_on_PC5  		PCICR |= (1 << PCIE1); 

//The reset control switch uses PC5 (PCINT13).This sets it up for weak pull up (WPU) with IPC 



/************************************************************************************************************************************/
#define Start_clock		    	TCNT0 = 0;  OCR0A = 0; TCCR0B = (1 << CS01);

//Initialises Timer 0 Output compare and counter values to zero, sets TO clock to 1MHz and starts it

#define wait_for_comms_tick \
OCR0A +=  Comms_clock;\
while (!(TIFR0 & (1 << OCF0A)));\
TIFR0 = 0xFF;

//Increment Timer 0 Output compare register A and wait until the value in the Timer 0 counter is the same 


#define wait_for_half_comms_tick \
OCR0A =  Comms_clock/2;\
while (!(TIFR0 & (1 << OCF0A)));\
TIFR0 = 0xFF;  



/************************************************************************************************************************************/
#define boot_reset_ctl_reg			0x3FC
#define 	set_Run_BL_bit			eeprom_write_byte((uint8_t*)boot_reset_ctl_reg, (eeprom_read_byte((uint8_t*)(0x3FC)) & (~4)));

//if the hex_text_programmer reads bit 2 of EPPPROM location 0x3FC as zero it will run the bootloader

#define PINB4_down	((PINB & 0x10)^0x10)
#define PINC5_down	((PINC & 0x20)^0x20)
#define PINC5_up	(PINC & 0x20)



/************************************************************************************************************************************/
#define  set_up_activity_leds \
DDRB |= (1 << DDB0) | (1 << DDB1);\
LED_1_off;\
LED_2_off;

#define LED_1_off	 	PORTB &= (~(1 << PB1));
#define LED_1_on	 	PORTB |= (1 << PB1);

#define LED_2_off	 	PORTB &= (~(1 << PB0));
#define LED_2_on	 	PORTB |= (1 << PB0);

#define Toggle_LED_1 \
if (PORTB & (1 << PB1)){LED_1_off;}\
else {PORTB |= (1 << PB1);}





