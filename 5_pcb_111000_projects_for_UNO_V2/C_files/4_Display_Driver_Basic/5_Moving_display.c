



int main (void){   
  char num_string[16];
  char display_string[8];
  char num_length;
  int p;
  
setup_HW;
if(watch_dog_reset == 1)String_to_PC_Basic("\r\nAgain"); else
String_to_PC_Basic("Enter number at keyboard\r\n");
num_length = Int_KBD_to_display_A_Local(num_string);

String_to_PC_Basic(num_string);
I2C_Tx_any_segment_clear_all();
for(int m = 0; m <=7; m++)display_string[m] = 0;

p = 0;
while(1){
display_string[0] = num_string[num_length - p%(num_length+1)]; p += 1;
I2C_Tx_8_byte_array(display_string);_delay_ms(100);

for(int m = 0; m < 7; m++){
display_string[7-m] = display_string[7-m-1];}

if(switch_1_down) break;}
SW_reset;}
