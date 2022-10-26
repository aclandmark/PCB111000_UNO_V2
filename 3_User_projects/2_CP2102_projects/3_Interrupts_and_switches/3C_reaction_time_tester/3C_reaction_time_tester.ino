
/*
 Very similar to Project 3B.  
 Press sw2 when the led bars line up to make the display flash
 */

#include "Reaction_time_tester_header.h"

int main (void)
{ unsigned int Random_num;
  unsigned int PORT_1, PORT_2;

 setup_328_HW_Basic_IO;
  while (switch_2_up)wdr();

  PORT_1 = 1;
  PORT_2 = 0x8000;
  while (1)
  { while (1)
    { Timer_T2_10mS_delay_x_m(1);
      while (switch_2_up)wdr();

      shift_PORT_1;
      shift_PORT_2;
      One_wire_Tx_2_integers(PORT_1, PORT_2);

      if (PORT_1 == PORT_2)
      { Timer_T2_10mS_delay_x_m(35);
        if (switch_2_up)break;
        else while (switch_2_down)wdr();
      }

      if (PORT_1 != PORT_2)
      {
        Timer_T2_10mS_delay_x_m(8);
      }
    }

    while (switch_2_up)
    {
      flash_display;
    }
  }
}



/*****************************************************************/
unsigned int PRN_16bit_GEN(unsigned int start){
unsigned int bit, lfsr;

if(!(start)) lfsr = (eeprom_read_byte((uint8_t*)(0x1FF)) << 8) + eeprom_read_byte((uint8_t*)(0x1FE));
else lfsr = start;
bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
lfsr = (lfsr >> 1) | (bit << 15);
if(!(start)){
eeprom_write_byte((uint8_t*)(0x1FF),(lfsr>>8));
eeprom_write_byte((uint8_t*)(0x1FE),lfsr);}

return lfsr;}




/******************************************************************/
