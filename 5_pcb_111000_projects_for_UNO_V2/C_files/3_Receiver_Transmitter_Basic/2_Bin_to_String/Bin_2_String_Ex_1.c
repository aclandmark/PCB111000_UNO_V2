


int main (void)              //Example 1 
  { int i = 0;              //Convert number to string
  int number = 12345;
  char s[12];
  setup_HW;
  //Set up watchdog timer to generate a reset
  wdt_enable(WDTO_120MS);
 while(switch_1_down)wdr();

  //Convert binary number to string
  while(1) {
    s[i++] = number % 10 + '0';
    number = number/10;
    if (!(number))break;
  }s[i] = '\0';

  //Print string out in reverse order
    for (int m = i; m > 0; m--)
    Char_to_PC_Basic(s[m - 1]);
  newline_Basic();
 //Generate a reset
 while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;}
  
  
  int main (void)               //Example 2
  {                             //Convert number to string
  long number = 2;				//Use local subroutines
 char num_string[12];
 char direction = 0;
 int str_len;
  
  setup_HW-basic;
 
while(number) {
str_len = Bin_to_string_Local(number, num_string);
 Num_string_to_PC_Local(num_string, str_len);
 if (!(direction))number = number *3/2;
else number = number *2/3;
  
  newline_Basic();
 waitforkeypress_Basic();
if (number >= 18000){direction ^= 1;
number = number *2/3;newline_Basic();}}
newline_Basic();SW_reset;
  return 1;}