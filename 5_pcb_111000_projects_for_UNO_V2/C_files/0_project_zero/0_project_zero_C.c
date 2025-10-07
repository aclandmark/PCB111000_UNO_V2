

  int main (void)     //Example 7 
{char next_LED = 0; //Select red/green  

setup_HW_Basic;   //combination of flashing leds

while(1){
if(switch_1_down)next_LED = (next_LED+1)%2;
while(switch_1_down);

switch(next_LED){
case 0:Toggle_LED_1;break;
case 1:Toggle_LED_2;break;}
 _delay_ms(100);}}
 
  
 
  int main (void)    //Example 8  Random display
{ unsigned int PRN = 1;
char timer_on = 50;
  setup_HW_Basic;
while(1){
    LEDs_off;
PRN = (PRN_8bit_GEN() % 4);
      switch (PRN) {
        case 0: LED_1_on; break;
        case 1: LED_2_on; break;
        case 2: LEDs_on; break;
        case 4:  LEDs_off; break; }
Timer_T0_10mS_delay_x_m(timer_on);
if (switch_1_down)timer_on = timer_on*2/3;
while(switch_1_down);
if(timer_on <= 2)break;}SW_reset;}
 


int main (void)  		//Example 9  
{ setup_HW_Basic;		//Red LED flashes 
int on_time = 50;		//with variable speed
 do{
    if(switch_1_down)on_time = on_time*2/3;
    while(switch_1_down);
  Toggle_LED_1;
  Timer_T0_10mS_delay_x_m(on_time);}
  while(on_time >= 2);
  SW_reset;}