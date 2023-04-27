

/************Define clock memory locations*******************************************************************************/
#define reset_ctl_reg                         0x3ED
#define Signal_flagged_WDTout                 eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0x10)
#define Signal_WDTout_with_interrupt          eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0x20)
#define clear_reset_ctl_reg                   eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0)
#define reset_ctl_reg_clear                   (eeprom_read_byte((uint8_t*)reset_ctl_reg) == (byte) ~0x0)


/***********Define clock display functions and modes***********************************************************************/
#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)


#define set_up_PCI \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));

#define enable_pci_on_sw1               PCMSK2 |= (1 << PCINT18);
#define disable_pci_on_sw1  PCMSK2 &= (~(1 << PCINT18));



/****************************************************************************************************************************/
#define  set_up_activity_leds                   /*Common cathode dual LEDs connected between B0 and B1 and 0V*/\
DDRB |= (1 << DDB0) | (1 << DDB1);               /*Ports B0 and B1 set to output*/\
LED_1_off;\
LED_2_off;

#define LED_1_off   PORTB &= (~(1 << PB1));       //Output low
#define LED_1_on    PORTB |= (1 << PB1);          //Output high

#define LED_2_off   PORTB &= (~(1 << PB0));
#define LED_2_on    PORTB |= (1 << PB0);

#define Toggle_LED_1 \
if (PORTB & (1 << PB1)){LED_1_off;}                 /*If output high set low*/\
else {PORTB |= (1 << PB1);}



/************************************************************************************************************************************/
#define determine_reset_source \
reset_status = 0;\
 if((watch_dog_reset == 1) && (reset_ctl_reg_clear))reset_status = 2;\
  if (eeprom_read_byte((uint8_t*)reset_ctl_reg) == (byte) ~0x10)reset_status = 4;\
 if (eeprom_read_byte((uint8_t*)reset_ctl_reg) == (byte) ~0x20)reset_status = 5;\
 clear_reset_ctl_reg;



/*Reset status:
1:    POR                                                       Only for UNO version of SW
2:    SW_reset
3:    External reset or press -r- at h/t/r/D prompt             Only for UNO version of SW
4     Flagged WDTout
5     WDTout with interrupt
6     WDT configured with interrupt but ISR missing.            Only for UNO version of SW
      Jumps to 0x0000 not x7000
 */




/*************************************************************************************************************************/
