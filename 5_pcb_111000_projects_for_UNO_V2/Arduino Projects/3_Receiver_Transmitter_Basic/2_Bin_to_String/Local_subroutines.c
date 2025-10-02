
void Char_to_PC_Basic(char);

void Int_to_PC_Basic (long);
void newline_Basic(void);
void String_to_PC_Basic(char*);




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





 void Re_order_array(int* num_array,int array_length){
int max_num;
int test_num, next_term=0;

newline_Basic(); newline_Basic();

  
test_num = num_array[0];
for(int n = 1; n < array_length; n++)
if(num_array[n] >= test_num) 
{test_num = num_array[n];}
max_num = test_num;

for(int p = 0; p < array_length; p++){
test_num = max_num;
for(int n = 0; n < array_length; n++)
{if((num_array[n] <= test_num) && (num_array[n] > next_term)) 
test_num = num_array[n];} 
Int_to_PC_Basic(test_num);String_to_PC_Basic("  ");

if(test_num == max_num)break;

next_term = test_num;}

newline_Basic(); newline_Basic();}



/***************************************************************************************/
