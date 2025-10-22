int main (void){
long Num_1;
char digits[8];
int counter = 0;
char num_string[12];
setup_HW_A;

if(watch_dog_reset == 1)Serial.write(message_2);
else Serial.write(message_1);
sei();
while((switch_1_down) || (switch_2_down) ||(switch_3_down));        //wait for switch release
Num_1 = number_from_IO();
do{
Int_Num_to_PC_A(++counter, num_string, '\t');
Int_Num_to_PC_A(Num_1, num_string, '\r');

I2C_Tx_long(Num_1);                                           //Sends number to the display
Timer_T0_10mS_delay_x_m(15);
while(switch_1_down)wdr();

Num_1 = (Num_1 / 2) *3;} 
while ((Num_1 < 99999999) && (Num_1 > -9999999));                   //Do some arithmetic

Serial.write("Press sw1 to continue\r\n");

while(switch_1_up)wdr();
Num_1 = (Num_1 / 3) *2; 
do{Num_1 = (Num_1 / 3) *2;                                           //Do the arithmetic in reverse
Int_Num_to_PC_A(--counter, num_string, '\t');
Int_Num_to_PC_A(Num_1, num_string, '\r');                                
I2C_Tx_long(Num_1);
Timer_T0_10mS_delay_x_m(15);
while(switch_1_down)wdr();}while (counter-1);
Serial.write("sw1 !\r\n");
while(switch_1_up)wdr();
while(switch_1_down)wdr();
setup_watchdog_for_UNO;
SW_reset;}