

int main (void)       //Example 1
  { unsigned int PORT_1;

  setup_HW_basic;
    PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1, PORT_1);
    Timer_T0_10mS_delay_x_m(12);
   PORT_1 = PORT_1 << 1;}
  SW_reset; }
  


int main (void)  //Example 2
  { unsigned int PORT_1;

  setup_HW_basic;
  wdt_enable(WDTO_30MS);
    PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1, PORT_1);
    Timer_T0_10mS_delay_x_m(12);
   PORT_1 = PORT_1 << 1;}
  while(1); }
	
	
	
	int main (void) 	//Example 3
  { unsigned int PORT_1;

  setup_HW_basic;
  wdt_enable(WDTO_30MS);
  PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers
  (PORT_1 << m, PORT_1 << m);
    Timer_T0_10mS_delay_x_m(12);
   }
for (int m = 14; m; m--)
{  I2C_Tx_2_integers
(PORT_1 << m, PORT_1 << m);
    Timer_T0_10mS_delay_x_m(12);}
  while(1); }