

int main (void){
long Num;


char strlength;

setup_HW_basic;

  String_to_PC_Basic("Enter hex number then x, X or y, Y or AK\r\n");  
   Num = Int_from_PC_local(0x10, digits);
           
   newline_Basic();
    
while(1){ 
  strlength = Long_Hex_and_Int_to_PC_Basic_local(0x10, Num);
for(int m = strlength; m < (30 - strlength); m++)Char_to_PC_Basic(' ');
strlength =  Long_Hex_and_Int_to_PC_Basic_local(10, Num);
for(int m = strlength; m < (30 - strlength); m++)Char_to_PC_Basic(' ');
  Long_Binary_to_PC_local(Num);    
  String_to_PC_Basic("\r\n");
  
  switch(waitforkeypress_Basic()){
   case 'x': Num= Num * 0x10;break;
   case 'y': Num= Num / 0x10;break;
    case 'X': Num= Num * 10;break;
   case 'Y': Num= Num / 10;break;
   case 'z': Num = Num * -1; break;
   default:SW_reset;break;    
  }}}
  