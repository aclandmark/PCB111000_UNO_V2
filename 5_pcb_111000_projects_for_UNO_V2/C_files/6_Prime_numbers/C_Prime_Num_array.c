int main (void){
long array_num=0;                                        //Zero for first array of 200 numbers, one for second array etc.
int array_size = 200, array_pointer=1;
long num_array[200], prime_num;  
char counter=9;
char num_string[12];
 
setup_HW;
String_to_PC_Basic("Prime number array\r\n1  ");

while(1){
array_num = prime_no_generator(array_num,array_size,  num_array);
while(array_pointer < array_size)
{prime_num = next_prime_number(&array_pointer, num_array, array_size); 
if (counter)                                   //Print them out 10 at a time
{if (counter == 10)
String_to_PC_Basic("\r\n");
if(prime_num)Int_to_PC_Basic(prime_num); 
Char_to_PC_Basic(' ');
counter -= 1;
if(!(counter)){counter = 10;}}}

String_to_PC_Basic("\r\n\r\nZero or next array or AOK  ");
if(waitforkeypress_Basic()=='0')array_num += 1;
else {String_to_PC_Basic("\t Array number?  ");
  array_num=Int_from_PC_Basic( num_string);}
array_pointer=0;
counter = 10;
newline_Basic();}}

   
   