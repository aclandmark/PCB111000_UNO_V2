
long Real_Num_from_PC_Local(char digits[], long* divisor, char * decimal_places){    
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

