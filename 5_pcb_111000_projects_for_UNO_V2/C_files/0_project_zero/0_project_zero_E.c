

  int main (void)       //Example 12    
{ int counter, ON_time;   //LED flasher with variable brightness

  setup_HW_Basic;
  ON_time = 160;
  
  LED_2_on;
   while (1) {
    drive_led(ON_time);  
  LEDs_off;
  Timer_T1_sub(T1_delay_100ms);
  if(switch_3_down){ON_time = ON_time * 2/3;
  if (ON_time == 1)ON_time = 160; }}}


void  drive_led(int ON_time){
  Start_timer_T1_sub(T1_delay_100ms);

  while (!(TIFR1 & (1 << TOV1))){
  
  if (Led_2_is_on) {
      LED_2_off;
      for (int m = 0; m <= (320 - ON_time); m++)
    _delay_us(6); }
    else {LED_2_on;
      for (int m = 0; m <= ON_time; m++)
    _delay_us(6);}}

    TIFR1 |= (1 << TOV1);}
