


#include "Local_subroutines.c"

unsigned int PRN;
unsigned char PRN_counter;

 int main (void)                //Example 13
  {int T1_period = 15;
  char direction;
  char seg_counter;
  setup_HW_basic;
  
  I2C_Tx_any_segment_clear_all();
  PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);
   direction=1;
  while(1){
     direction ^= 1;
   seg_counter=0;
 PRN = update_display
 (T1_period, direction, seg_counter, PRN);
 if(switch_1_down); 
 else T1_period = Adjust_T1_period(T1_period);}}
 
 
 
 
 int main (void)               //Example 14
{ unsigned int PRN;
  unsigned char PRN_counter;

  setup_HW_basic;
  wdt_enable(WDTO_30MS);
  PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);
  while (1)
  { PRN = PRN_16bit_GEN (PRN, &PRN_counter);
    I2C_Tx_2_integers(PRN, (PRN << ((PRN % 2) + 1)));
    Timer_T0_10mS_delay_x_m(10);
  }}