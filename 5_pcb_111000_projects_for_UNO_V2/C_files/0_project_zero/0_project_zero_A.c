int main (void)  	//Example 1  Red LED flashes
{  setup_HW_Basic;
    LED_1_on;
  SW_reset;}


int main (void)    //Example 2  Red LED flashes
{  setup_HW_Basic;
if(switch_3_down){wdt_enable(WDTO_120MS); _delay_ms(60);}
else wdt_enable(WDTO_30MS);
 LED_1_on;
 while(1);}


int main (void)  	//Example 3  Red LED flashes
{ setup_HW_Basic;
  while(1){
  Toggle_LED_1;
  _delay_ms(50);}}


int main (void)  	//Example 4  Red LED flashes 
{  setup_HW_Basic;	//when switch_1 is pressed
  while (1) {
    while (switch_1_up);
    {Toggle_LED_1; }
    Timer_T0_10mS_delay_x_m(20);  }}

