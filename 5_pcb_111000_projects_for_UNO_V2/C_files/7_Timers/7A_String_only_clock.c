


int main (void){
char User_response;

setup_HW_A;

Serial.write("Press 'R' to enter time or 'r' to start at time zero  ");
User_prompt_A;

if(User_response == 'R')set_time();
else {reset_clock_1;}
I2C_Tx_8_byte_array(digits);  

Serial.write("AK to start\r\n");
waitforkeypress_A();
while(1){Timer_T0_10mS_delay_x_m(20);Inc_display();Inc_display();}}