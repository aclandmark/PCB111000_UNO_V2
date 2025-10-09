

void Char_to_PC_Basic(char);
unsigned char PRN_8bit_GEN(void);
unsigned int PRN_16bit_GEN(unsigned int, unsigned char *);
void Int_to_PC_Basic (long );



int Bin_to_string_Local(long number, char *s){
  int i = 0; 
 while(1) {
    s[i++] = number % 10 + '0';
    number = number/10;
    if (!(number))break;
  }s[i] = '\0';
   return i;}




void Num_string_to_PC_Local(char *s, int str_len){
 for (int m = str_len; m > 0; m--)
 Char_to_PC_Basic(s[m - 1]);}





 unsigned int new_random_number 
 (unsigned int * sixteen_bit_PRN, unsigned char * PRN_counter)
  
  {unsigned int Eight_bit_PRN; 
 
  * sixteen_bit_PRN = PRN_16bit_GEN (* sixteen_bit_PRN, PRN_counter);
    Eight_bit_PRN = PRN_8bit_GEN();

switch(Eight_bit_PRN%5)
 {case 0: return *sixteen_bit_PRN%10; break;
  case 1: return *sixteen_bit_PRN%100; break;
  case 2: return *sixteen_bit_PRN%1000; break;
  case 3: return *sixteen_bit_PRN%10000; break;
  case 4: return *sixteen_bit_PRN%100000; break;}}



/***************************************************************************************/
