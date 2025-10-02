

void Short_Int_to_PC_Local (char, char, char);
void Char_to_PC_Basic(char);
void String_to_PC_Basic(const char s[]);
void Short_binary_to_PC_Local(int);
void newline_Basic(void);

//char digit;
//char mode;


void update_screen(char mode,char digit){

Short_Int_to_PC_Local (10,digit, mode);
Char_to_PC_Basic ('\t');  
Short_Int_to_PC_Local (16,digit, mode);
Char_to_PC_Basic ('\t');
Short_binary_to_PC_Local(digit);

if(mode == 's'){
Char_to_PC_Basic ('\t');
Short_binary_to_PC_Local((~digit) + 1);
Char_to_PC_Basic ('\t');
Short_Int_to_PC_Local (16, (~digit)+1, + mode);
Char_to_PC_Basic ('\t');
Short_Int_to_PC_Local (10, (~digit)+1, + mode);}

newline_Basic();}


void Short_binary_to_PC_Local(int x){             //check with paper and pencil
for (int m = 0; m<8; m++){
  if(x & (1 << (7-m))) Char_to_PC_Basic('1');        //test each bit in turn starting with the most significant
  else Char_to_PC_Basic('0');   
  if(m%4==3)String_to_PC_Basic("  ");}}



void Short_Int_to_PC_Local (char radix, char number, char mode)
{ int i = 0;
   char s[12];
   char sign = '+';
   unsigned char num;
   
    //num = (byte)number;
   num = (unsigned)number;    //Should really be unsigned char
   if(radix == 10){  if ((mode == 's') && (number < 0)){num = -number; sign = '-';}}
   do
  { s[i] = num % radix;// + '0';
if (s[i] < 10)s[i] += '0'; else s[i] += '7';
   i += 1; 
  }
  while ((num = num / radix) > 0);
  s[i] = '\0';
  if (sign == '-')Char_to_PC_Basic('-');
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}
  /************************************************************************************************************************/
