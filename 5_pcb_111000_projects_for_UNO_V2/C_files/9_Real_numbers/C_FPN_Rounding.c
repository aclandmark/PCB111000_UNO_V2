void Real_num_to_PC_no_rounding_Local(long int_num, int twos_expt, char decimal_places, char * num_string){
long  divisor;
long Div, mod;

Serial.write('\t');
 
 if (twos_expt < 0){
 divisor = 1;
 twos_expt = twos_expt * (-1);

 while (twos_expt >= 31){twos_expt -= 1; int_num = int_num >> 1;}
 for (int m = 0; m < twos_expt; m++) divisor *= 2;

 real_divide(int_num, divisor, &Div, &mod); 
 Int_Num_to_PC_A(Div, num_string, ' ');
 Serial.write('.');
 for(int m = 0; m < decimal_places; m++){

 while (mod >= 0x7FFFFFFF/10){mod = mod/10; divisor /= 10;}
  
 real_divide(mod*10, divisor, &Div, &mod);
 Serial.write(Div + '0'); }}

else {while(twos_expt) {int_num *= 2; twos_expt -= 1;}Int_Num_to_PC_A(int_num, num_string, ' ');}}