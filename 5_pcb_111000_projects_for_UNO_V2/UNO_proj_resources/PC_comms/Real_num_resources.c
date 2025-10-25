
void real_divide(long, long, long *, long *) ;


long Real_Num_from_PC(char digits[], long* divisor, char * decimal_places){    
char keypress;
int digit_counter = 0;
newline;

*divisor = 0;
*decimal_places = 0;
while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break; 
if (keypress == '.'){*divisor = 1;
Serial.write('.');}
if (decimal_digit_A(keypress))
{ Serial.write(keypress);
  digits[digit_counter++] = keypress;
if (*divisor) {*divisor *= 10;
*decimal_places += 1;}}}
if(!(*divisor))*divisor = 1;
digits[digit_counter] = '\0' ;                                                 
return atol (digits);}



/****************************************************************************************************/
void print_fpn_as_binary(float fpn, char sym){
  unsigned long mask = (unsigned long)0x80000000;
long int_num;

int_num = *(long*)&fpn; 
for (int m = 0; m <= 31; m++){

  switch(m)
 { case 1:
  case 5:
  case 9:
  case 13:
  case 17:
  case 21:
  case 25:
  case 29: Serial.write(' ');}  
  
  if ((unsigned long)int_num & (mask)) Serial.write('1'); else Serial.write('0');
 mask = mask >> 1;
  } Serial.write(sym);Serial.write('\t');}



/******************************************************************************************************/
void print_long_as_binary(long Num, char sym){
  unsigned long mask = (unsigned long)0x80000000;

for (int m = 0; m <= 31; m++){
  if (!(m%4)) Serial.write(' ');
  if ((unsigned long)Num & (mask)) Serial.write('1'); else Serial.write('0');
 mask = mask >> 1;
  } Serial.write(sym);newline;}
  
  
  
  /******************************************************************************************************/
  float assemble_fpn(long int_result,int twos_expt){
float fpn;

twos_expt += 24;                                                        //Shift binary point from right hand end to left hand end
twos_expt += 126;                                                       //Floating point number convention                
int_result = int_result  &  (~((unsigned long)0x80000000 >> 8));        //Clear bit 23 (which is always 1)
int_result = int_result | ((long)twos_expt << 23);                      //Exponent can now occupy bits 23 to 30 (bit 31 reserved for sign)
fpn =  *(float*)&int_result;  
return fpn;}
  
  
  
  /******************************************************************************************************/
  long int_divide(long A, long B, int * binary_pt_counter)    
  {
    long Div, mod;
    long Int_result;

 *binary_pt_counter = 0;
 
real_divide(A, B, &Div, &mod);                      //Perform the first division to get the integer result plus remainder
 Int_result = Div;
while (!(Int_result & 0x1000000))                   //Reserve 25 bits for the result of the division      
{real_divide(mod*2, B, &Div, &mod);                 //Double the remainder and repeat the division.
 Int_result = Int_result <<1;                       //For each division shift result one place left
    if (Div) Int_result = Int_result | 1;           //If the result was one place a one in the bit just vacated
    *binary_pt_counter -= 1;}                            //The number of bits to the right of the binary point
   Int_result += 1;                                 //Add 1 to round up the result
   Int_result = Int_result>> 1;                     //Reduce number to 24 bits
  *binary_pt_counter += 1;
   
  return Int_result; } 



/*****************************************************************************************************************************/
void real_divide(long A, long B, long *Div, long *mod)      
  {*Div = A/B;
  *mod = A%B;}



  
  /*******************************************************************************************************/