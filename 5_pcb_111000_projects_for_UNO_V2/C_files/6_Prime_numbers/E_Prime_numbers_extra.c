


/***************************************************************************************************************************************************/
ISR(PCINT2_vect){ 
if((switch_1_up) &&(switch_2_up)) return; 

if(switch_1_down){_delay_ms(10);
while(switch_1_down)wdr();return;}                           //Press sw_1 to pause display

if(switch_2_down)
{while (switch_2_down){start_1+=2; 
Int_to_PC_Basic(start_1);                           //Hold sw_3 down to increment start point
Char_to_PC_Basic(' ');
Timer_T1_sub(T1_delay_1sec);}                               //for random number generator
newline_Basic(); 
return;}}



/***************************************************************************************************************************************************/
void factors_to_display(int * factor, int m){
  
  for(int n=0; n<m; n++){I2C_Tx_any_segment_clear_all();
  Timer_T0_10mS_delay_x_m(10);
  I2C_Tx_long(factor [n]);
  while (switch_3_up)wdr();
  while (switch_3_down)wdr();}}

