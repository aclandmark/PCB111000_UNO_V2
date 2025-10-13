int main (void){
long array_num=0;                                        //Zero for first array of 200 numbers, one for second array etc. 
long array_size = 200, array_pointer=1;
long array_of_nums[200], prime_num;  
int counter=0;
long number;
char num_string[12];

 setup_HW;
if(watch_dog_reset == 1)
String_to_PC_Basic("\r\n?");
else 
String_to_PC_Basic("\r\nEnter number  ");
number = Int_from_PC_Basic( num_string);
while(1){
array_num = prime_no_generator(array_num, array_size,  array_of_nums);
while(array_pointer < array_size)
{prime_num = next_prime_number(&array_pointer, array_of_nums, array_size); 

if(prime_num){
counter += 1; if (!(counter%50)){counter = 0;Char_to_PC_Basic('.');}
while (!(number%prime_num))
{String_to_PC_Basic("A ");Int_to_PC_Basic(prime_num); 
Char_to_PC_Basic(' '); 
number /= prime_num;}
if (number ==1)SW_reset;}}
array_num += 1;
array_pointer=0;}}