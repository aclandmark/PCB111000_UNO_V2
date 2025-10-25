
void shift_24_bit_num_one_place_left(long high_bytes, long low_bytes, long * high_res, long * low_res){
*high_res = high_bytes << 1;  
if (low_bytes & 0x800000)*high_res = *high_res | 1;
*low_res = low_bytes << 1;
*high_res = *high_res & (~(0xFF000000));
*low_res = *low_res & (~(0xFF000000));}



/***************************************************************************************************************************/
void add_48_bit_nums(long A_high, long A_low, long *sum_high, long *sum_low){
  char A, B;
   char sum;
  char carry = 0;
  
  for(int m = 0; m < 24; m++) {
  if ((0x800000 >> (23 - m)) & A_low) A= 1; else A = 0;
  if((0x800000 >> (23 - m)) & *sum_low)B= 1; else B = 0;
  if(bit_sum(A, B, carry))*sum_low |= (0x800000 >> (23 - m)); else *sum_low &= (~(0x800000 >> (23 - m)));
  carry = Carry_out(A, B, carry); }

for(int m = 0; m < 24; m++){

 if ((0x800000 >> (23 - m)) & A_high) A= 1; else A = 0;
  if((0x800000 >> (23 - m)) & *sum_high)B= 1; else B = 0;
  if(bit_sum(A, B, carry))*sum_high |= (0x800000 >> (23 - m)); else *sum_high &= (~(0x800000 >> (23 - m)));
  carry = Carry_out(A, B, carry); }}