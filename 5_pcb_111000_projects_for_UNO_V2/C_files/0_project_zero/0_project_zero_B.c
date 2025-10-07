


int main (void)    //Example 5 
{setup_HW_Basic;  //Choose colour of static LED
  
  while (1) {
    while ((switch_1_up) && (switch_2_up));
    if (switch_1_down)
    {Toggle_LED_1;}
    else
    {Toggle_LED_2;}
     while (switch_3_up);}}


 int main (void)    //Example 6  Random display
{ unsigned int PRN = 1;
  setup_HW_Basic;
  PRN = (PRN_8bit_GEN() % 50);
  Timer_T0_10mS_delay_x_m(PRN);

if (eeprom_read_byte((uint8_t*)(0x3DF))%2)
{LED_1_on;}
else {LED_2_on;}

eeprom_write_byte((uint8_t*)(0x3DF), 
(eeprom_read_byte((uint8_t*)(0x3DF))+1));
SW_reset;}
  