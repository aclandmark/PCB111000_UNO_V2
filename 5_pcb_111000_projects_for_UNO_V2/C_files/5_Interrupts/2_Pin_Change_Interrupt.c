
int main (void)
{ setup_HW;
  _delay_ms(10);
  sei();
  set_up_pci_on_sw3;
  enable_pci_on_sw3;

  mask = 0xFFFF;                                        //No leds shot down yet
  
  while (mask)                                          //Mask is zero when last led has been shot down
  { switch_control = 1;                                 //Disable switch and 
    Timer_T0_10mS_delay_x_m(20);                         //pause at start of each scan
    scan = 1;                                           //Enable scan
    PORT_1 = 1;                                         //Illuminate RH leds at start of new scan
    while (scan)
    { if (PORT_1 == 1)switch_control = 0;               //Enable switch 2 at start of new scan
      I2C_Tx_2_integers
      (PORT_1 & mask, (~mask) ^ PORT_1);                 //Update display as leds are shot down
      Timer_T0_10mS_delay_x_m(10);
      if (PORT_1 & 0x8000)scan = 0;                     //Most LH leds illuminated: Pause scan
      else
        PORT_1 = (PORT_1 << 1);                         //Illuminate next set of LEDS moving left 
    }
  }
  I2C_Tx_2_integers(0, 0xFFFF);                    //Pause display before starting again
  Timer_T0_10mS_delay_x_m(200);
  SW_reset;
}