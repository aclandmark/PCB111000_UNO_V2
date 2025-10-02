

int main (void) {
long Num_A, Num_B, Num_C;
char digits[8]; 

setup_HW_basic;
for(int m = 0; m <= 7; m++)digits[m] = 0;
I2C_Tx_any_segment_clear_all();

if(watch_dog_reset != 1)String_to_PC_Basic
("\r\nEnter 2 positive numbers to be multiplied\r\n\
Then press sw1\r\n");
else String_to_PC_Basic("\r\n");

while(switch_1_down);

Num_A = Int_KBD_to_display_Local(digits);
String_to_PC_Basic(" times ");
Num_B = Int_KBD_to_display_Local(digits);
String_to_PC_Basic("  =  ");

Num_C = 0;
for (int m = 0; m <=15; m++)
{
  if (Num_B & (1 << m))Num_C = Logic_Add(Num_C, (Num_A << m));
}

for(int m = 0; m <= 7; m++)digits[m] = 0;
 I2C_Tx_any_segment_clear_all();

digits[0] = Num_A;
digits[1] = Num_B;
digits[2] = Num_C;
I2C_Tx_BWops(digits);
Long_Hex_and_Int_to_PC_Basic (10, Num_C);

while(switch_1_up);
SW_reset;}