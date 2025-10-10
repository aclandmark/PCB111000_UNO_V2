

void Int_to_PC_Basic(long);
char waitforkeypress_Basic(void);
extern volatile char num_present;


/*******************************************************************************************************/
char Int_to_String_Basic (long number, char * s)
{ int i = 11;                                               //Save characters starting from the end of the array rather than the begining
for(int m = 0; m <= 11; m++)s[m] = 0;
   do
  { *(s+i) = number % 10 + '0'; i-=1;  }
  while ((number = number / 10) > 0);
 
if (i >= 0){                                                //Shift the characters from the end of the array to the beginning
for(int m = 1; m <=(11-i); m++)*(s + m-1) = *(s + m+i); 
*(s + 11-i) = '.';}
 return (12-i);}


/*******************************************************************************************************/
void Num_string_from_KBD_Local(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    array_ptr += 1;}
  *array_ptr = '\0'; num_present = 1;}




/*******************************************************************************************************/
void root_computation(long Num, char * result_as_string){
long result = 1, result_old;
long start_value = 2, product = 1;
char str_ptr;
 
/**********************Obtain integer part of the result****************************/
if((product = start_value * start_value) > Num)result = 1; 
else{
while(1){
product = start_value * start_value; 
if (product <= Num){result = start_value;} 
else break;
start_value += 1;
product = 1.0;}}
str_ptr = Int_to_String_Basic(result, result_as_string); //Char_to_PC_Basic('.');

/*****Improve iteration starting with one decimal place then 2, 3......up to 6 in all****/
for(int p = 1; p <= 6; p++)
{Num *= 100;
  result *= 10;
  result_old = result;
product = 1.0;
start_value = result + 1;
while(1){
 product = start_value * start_value;
if (product <= Num){result = start_value;}
else break;
start_value += 1;
product = 1.0;}
*(result_as_string + str_ptr) = (start_value-result_old -1 + '0');str_ptr += 1;}
*(result_as_string + str_ptr) = '\0';}






 /*********************************************************************************************/
