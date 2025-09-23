


int main (void)      //Example 11
  { unsigned int PORT_1 = 1;
  char m = 1;
  char overflow = 0;
  setup_HW_basic;
  wdt_enable(WDTO_30MS);
  while(switch_1_down)wdr();
  
  while (1)
  { I2C_Tx_2_integers(PORT_1, ~PORT_1);
    
    Timer_T0_10mS_delay_x_m(8);
    
    if (m <= 5)
    {
      PORT_1 |= (PORT_1 << 1);
      m += 1;
    wdr();}
    else PORT_1 = PORT_1 << 1;
    if (overflow)PORT_1 |= 1;
    if (PORT_1 & 0x8000) overflow = 1;
    else overflow = 0;
    while(switch_1_down);  }}
	
	
	int main (void)   //Example 12
  { unsigned int random_num;
  unsigned char PRN_counter;
  long PORT_1 = 1, PORT_2 = 1;

  setup_HW_basic;
  wdt_enable(WDTO_30MS);
  PRN_counter = 0;
  random_num = PRN_16bit_GEN (0, &PRN_counter);
  while (1)
  { for (int m = 0; m < random_num % 3; m++)
    { if (!(PORT_1 = ((PORT_1 * 2) % 0x10000)))
        PORT_1 = 1;}
    if (!(PORT_2 = ((PORT_2 * 2) % 0x10000)))
	PORT_2 = 1;
    I2C_Tx_2_integers(PORT_1, PORT_2);
    Timer_T0_10mS_delay_x_m(8);
    random_num = PRN_16bit_GEN 
	(random_num, &PRN_counter);}}  