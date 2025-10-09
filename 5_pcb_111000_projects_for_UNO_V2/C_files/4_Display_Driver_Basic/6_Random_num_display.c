


  
  int main (void)
  { unsigned int sixteen_bit_PRN;
    unsigned int random_num[2];
  unsigned char PRN_counter=0;
  char display_string[8], num_string[8];
  char num_length;
    int num_counter=0;
    

  setup_HW;
  
  PRN_counter = 0;
  random_num[0] = PRN_16bit_GEN (0, &PRN_counter);
   
sixteen_bit_PRN = random_num[0];
  
  while (1)       
  { num_length = Bin_to_string_Local(random_num[num_counter%2], num_string);

    for(int n = 0; n <= num_length; n++){
    display_string[0] = num_string[num_length - n];
  I2C_Tx_8_byte_array(display_string);_delay_ms(100);
   
   for(int m = 0; m < 7; m++){
display_string[7-m] = display_string[7-m-1];}
 
Timer_T0_10mS_delay_x_m(2);}
 
random_num[(num_counter+1)%2] = new_random_number  
 (&sixteen_bit_PRN, &PRN_counter);

num_counter += 1;}}