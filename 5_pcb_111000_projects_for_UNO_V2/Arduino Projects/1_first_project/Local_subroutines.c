
char display_bkp[7];
char digit_num;
extern  unsigned char PRN_counter;


unsigned int PRN_16bit_GEN(unsigned int, unsigned char*);
void I2C_Tx_any_segment(char, char);
void Timer_T0_10mS_delay_x_m(int m);


#include <avr/wdt.h>
#define wdr()  __asm__ __volatile__("wdr")

#define switch_1_down  ((PIND & 0x04)^0x04)


int Adjust_T1_period(int T1_period){
  if (T1_period <= 1)T1_period = 15;
  else
  T1_period = T1_period *12/15;
  return T1_period;}



  void backup_the_display(char segment, char digit_num)
  { display_bkp[segment - 'a'] =
    display_bkp[segment - 'a'] ^ (1 << digit_num);}



unsigned long update_display (int T1_period, char direction, char seg_counter, unsigned long PRN)
{char letter;

  while (seg_counter < 56) {
      letter = (PRN % 7) + 'a';
      PRN = PRN_16bit_GEN (PRN, &PRN_counter);
      digit_num = (PRN % 8);
      if ((!(direction)) && (display_bkp[letter - 'a'] & (1 << digit_num))) {
        PRN_counter -= 1;
        continue; }

      if ((direction) && (!(display_bkp[letter - 'a'] & (1 << digit_num)))) {
       PRN_counter -= 1;
       continue;}
       
      I2C_Tx_any_segment(letter, digit_num);
      backup_the_display(letter, digit_num);
      seg_counter += 1;
      Timer_T0_10mS_delay_x_m(T1_period);}
      return PRN;}




    /*************************************************************************************************/
