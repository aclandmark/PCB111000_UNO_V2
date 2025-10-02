

int main (void){               //Example 1
  char num_string[12];
  long num;
setup_HW_basic;
if(watch_dog_reset == 1)watch_dog_reset = 0; else
String_to_PC_Basic("Enter number at keyboard\r\n");
Num_string_from_KBD_Local(num_string);
Char_to_PC_Basic('\t');
num = Askii_to_binary_Local(num_string);
Int_to_PC_Basic(num/2);
newline_Basic();
SW_reset;
return 1;}




int main (void){                //Example 2
  char num_string[12];
  long num = 2;
  long old_num = 2;
setup_HW_basic;
String_to_PC_Basic("Multiply number by 2 and enter it\r\n");
String_to_PC_Basic("2\r\n");
while(1){
  
Num_string_from_KBD_Local(num_string);
num = Askii_to_binary_Local(num_string);

if(num == old_num * 2)
{newline_Basic();old_num = num;}
else Char_to_PC_Basic('?');
}
SW_reset;
return 1;}