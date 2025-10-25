

void Format_num_string(char* num_string, char * digits, char pre_dp, int Exp, char mode ){

for(int m = 0; m <=7; m++)digits[m] = num_string[7-m];

  for (int m = 0; m < (7-pre_dp); m++) digits[m] = digits[m + 1];
  digits[7 - pre_dp] = '.'; 

if((mode == '0') || (mode == 'y')){
if (Exp > 0){
if (Exp < 10) {digits [1] = 'E'; digits[0] = Exp + '0';}
if (Exp >=10) {digits [2] = 'E'; digits[0] = (Exp%10) + '0'; digits[1]= (Exp/10) + '0';}}

if (Exp < 0){Exp *= (-1);
if (Exp < 10) {digits [2] = 'E'; digits[1] = '-'; digits[0] = Exp + '0';}
if (Exp >=10) {digits [3] = 'E'; digits[2] = '-'; digits[0] = (Exp%10) + '0'; digits[1]= (Exp/10) + '0';}}}}

