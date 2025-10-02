


int main (void)  
  { 
    char num_string[12];
    long  num;
    long m = 1;
 
 setup_HW_basic;

if(!(watch_dog_reset))String_to_PC_Basic(message_1);
else String_to_PC_Basic(message_2);
  
num = Int_from_PC_Local(num_string);

newline_Basic();
Int_to_PC_Basic(num);
String_to_PC_Basic("\t");

do{
if(!(num%m)){Int_to_PC_Basic(m);}
m += 1; if(!(m%10000))String_to_PC_Basic(".");}while(m < num);

SW_reset;
return 1;  }