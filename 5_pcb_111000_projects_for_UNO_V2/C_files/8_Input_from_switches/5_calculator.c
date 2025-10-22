int main (void){
  float x1, x2;
  float power;
char op;
  setup_HW_A;
if(!(watch_dog_reset))
{Serial.write(message_1);}
else {Serial.write("\r\nAgain\r\n"); watch_dog_reset = 0;}
 x1 = fpn_from_IO();
 while(switch_2_down);
 I2C_FPN_to_display(x1);
 Sc_Num_to_PC_A(x1,1,6 ,'\r');
  
while(1){
 while(switch_1_up);
op = 0;
I2C_Tx_any_segment_clear_all();;
while(switch_1_down)
{op = op%8;
I2C_Tx_any_segment('d', 7- op);
op += 1;
Timer_T0_10mS_delay_x_m(40);
I2C_Tx_any_segment_clear_all();}

switch(op){
case 1:   case 2:
case 3:   case 4:
  x2 = fpn_from_IO(); while(switch_2_down); break;
  case 5:   x1 = pow(x1, 2);    break;
  case 6:   x1 = pow(x1, 0.5);  break;
  case 7:   x1 = 1.0/x1;        break;
  case 8:   SW_reset;           break;}
switch(op){
case 1:   x1 = x1 + x2; break;
case 2:   x1 = x1 - x2; break;
case 3:   x1 = x1 * x2; break;
case 4:   x1 = x1 / x2; break;}
Sc_Num_to_PC_A(x1,1,6 ,'\r');
I2C_FPN_to_display(x1);}
while(switch_1_up);
SW_reset;}