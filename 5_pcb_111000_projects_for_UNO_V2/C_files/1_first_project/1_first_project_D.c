



int main (void)     //Example 9
  { unsigned int PORT_1, PORT_2;
	unsigned int m = 0, n = 0;
  setup_HW_basic;
  PORT_1 = 1;
  PORT_2 = 0x8000;
  while (1)
  { I2C_Tx_2_integers
  (PORT_1 << m, PORT_2 >> m);
    _delay_ms(60);
    if (!(n)) m += 1;
    if (m == 16)n = 1;
    if (n == 1)m -= 1;
    if (m == 0)n = 0;
  }
  return 1;}




int main (void)        //Example 10
  { unsigned int PORT_1 = 1;
  char m = 1;
  char overflow = 0;

  setup_HW_basic;
  while (1)
  { I2C_Tx_2_integers
  (PORT_1, ~PORT_1);
    
    _delay_ms(80);
    
    if (m <= 5)
    {
      PORT_1 |= (PORT_1 << 1);
      m += 1;
    wdr();}
    else PORT_1 = PORT_1 << 1;
    if (overflow)PORT_1 |= 1;
    if (PORT_1 & 0x8000) overflow = 1;
    else overflow = 0;  }}  