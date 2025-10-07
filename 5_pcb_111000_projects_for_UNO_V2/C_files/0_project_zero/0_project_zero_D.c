
   
     int main (void)       //Example 10    
{ int counter, ON_time;   //Variable brightness

  setup_HW_Basic;
  counter = 0;
  ON_time = 160;
  
  LED_2_on;
  while (switch_3_up);

  while (1) {
    if (Led_2_is_on) {
      LED_2_off;
      for (int m = 0; m <= (320 - ON_time); m++)
    _delay_us(6); }
    else {LED_2_on;
      for (int m = 0; m <= ON_time; m++)
    _delay_us(6);}
    counter += 1;
    if ((counter >= 100)  && (switch_3_down)) {
      counter = 0;
      ON_time = ON_time * 2/3; }
    if (ON_time == 1) {
      ON_time = 160;
      while (switch_3_down); }}}
	  
	  
	  
	  int main (void)		//Example 11
{unsigned char next_LED;	//LEDs in rotation

setup_HW_Basic;
_delay_ms(100);
next_LED = eeprom_read_byte((uint8_t*)(0x3DF))%3;

switch(next_LED){
case 0:LED_1_on;break;
case 1:LED_2_on;break;
case 2:LEDs_on; break;}

if (switch_1_down)next_LED = 1;
if (switch_2_down)next_LED = 2;
if (switch_3_down)next_LED = 3;

eeprom_write_byte((uint8_t*)(0x3DF), next_LED);
 _delay_ms(40);
SW_reset;}
  
  