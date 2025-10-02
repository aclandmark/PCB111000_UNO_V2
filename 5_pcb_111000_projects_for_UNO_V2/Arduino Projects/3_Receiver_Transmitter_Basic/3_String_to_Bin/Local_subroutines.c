


void Int_to_PC_Basic (long);
void newline_Basic(void);


void Re_order_array(int* num_array,int array_length){
int max_num;
int test_num, next_term=0;
  
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
Int_to_PC_Basic(test_num);newline_Basic();

if(test_num == max_num)break;

next_term = test_num;}}



/*******************************************************************************************/
