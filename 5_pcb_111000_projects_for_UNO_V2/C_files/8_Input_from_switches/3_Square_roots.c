

int main (void){
long Num_1;

char digits[8];
char result[8];
int counter = 0;

setup_HW_A;
if(watch_dog_reset == 1)Serial.write(message_2);
else Serial.write(message_1);

sei();
while((switch_1_down) || (switch_2_down) ||(switch_3_down));        //wait for switch release

Num_1 = number_from_IO();
I2C_Tx_long(Num_1);                                           //Sends number to the display
Timer_T0_10mS_delay_x_m(15);

root_computation(Num_1, digits);
    Serial.write(digits); 
    for(int m = 0; m < 8; m++)result[m] = digits[7-m];
    I2C_Tx_8_byte_array(result);          

while(switch_2_up);
SW_reset;}