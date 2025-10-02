

char bit_sum (char, char, char);
char Carry_out (char, char, char);
long High_bits(long);
long Low_bits(long);
long Logic_Add(long, long);

/*************************************************************************************/


long Logic_subtract_Num1_Num_2(long Num_1, long Num_2){

  Num_2 = (~Num_2) + 1;

  return Logic_Add(Num_1, Num_2);}





long Logic_Add(long Num_1, long Num_2){
long Result_L = 0, Result_H = 0;
char Carry = 0;
char A, B;
long Result = 0;

for (int m = 0; m <=15; m++){
if (Low_bits(Num_1) & (1 << m)) A=1; else A=0;
if (Low_bits(Num_2) & (1 << m)) B=1; else B=0;

if (bit_sum (A, B, Carry))
Result_L |= (1 << m);
Carry = Carry_out(A, B, Carry);}

for (int m = 0; m <=15; m++){
if (High_bits(Num_1) & (1 << m)) A=1; else A=0;
if (High_bits(Num_2) & (1 << m)) B=1; else B=0;

if (bit_sum (A, B, Carry))
Result_H |= (1 << m);
Carry = Carry_out(A, B, Carry);}

return (((unsigned long)Result_H) << 16)  | (Result_L & 0xFFFF);}




/*************************************************************************************/
char bit_sum (char A, char B, char Carry_in)
{return (A^B)^Carry_in;}

char Carry_out (char A, char B, char Carry_in)
{if (Carry_in)return (A|B);
else return (A&B);}




/*************************************************************************************/
long High_bits(long Num)
{return Num >> 16;}


long Low_bits(long Num)
{return Num & 0xFFFF;}
