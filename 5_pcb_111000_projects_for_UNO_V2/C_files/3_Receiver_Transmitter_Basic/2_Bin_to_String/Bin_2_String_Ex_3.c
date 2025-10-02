

int main (void)                          //Example 5 
  { unsigned int random_num; 
    unsigned char PRN_counter;
    unsigned int Short_random_number; 
  char num_string[12];
 int str_len;
  int num_array[40];

  setup_HW_basic; 
  PRN_counter = 0;
  random_num = PRN_16bit_GEN (0, &PRN_counter);
  Short_random_number = PRN_8bit_GEN();

while(1){
  
 for(int m = 0; m <40; m++){
switch(Short_random_number%4)
 {case 0:num_array[m] = random_num%10; break;
  case 1:num_array[m] = random_num%100; break;
  case 2:num_array[m] = random_num%1000; break;
  case 3:num_array[m] = random_num%10000; break;}

str_len = Bin_to_string_Local(num_array[m], num_string);
 Num_string_to_PC_Local(num_string, str_len);
waitforkeypress_Basic(); 
Char_to_PC_Basic(' ');

random_num = PRN_16bit_GEN (random_num, &PRN_counter);
 Short_random_number = PRN_8bit_GEN();}
 
 
 Re_order_array(num_array,40);
}
return 1;}