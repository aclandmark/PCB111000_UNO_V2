


/**********************************************************************************************************************************************************************************/
void Int_to_PC_Basic (long number)
{ int i = 0;
  char s[12];
   do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');}



/**********************************************************************************************************************************************************************************/
long Int_from_PC_Basic(char digits[]){
char keypress;
for(int n = 0; n<=7; n++) digits[n] = 0; 

do
{keypress =  waitforkeypress_Basic();} 
while (!(decimal_digit_Basic(keypress)));                                      //(non_decimal_char(keypress));  //Not -,0,1,2,3,4,5,6,7,8 or 9
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);

while(1){
if ((keypress = wait_for_return_key_Basic())  =='\r')break;
if (decimal_digit_Basic (keypress))                                           //012345678or9  :Builds up the number one keypress at a time
{for(int n = 7; n>=1; n--)
digits[n] = digits[n-1];                                                //Shifts display left for each keypress
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);}}
            
return I2C_displayToNum();}




/**********************************************************************************************************************************************************************************/
char wait_for_return_key_Basic(void){                  						//Returns key presses one at a time
char keypress,temp;
while(1){																//Remain in while loop until a character is received
if (isCharavailable_Basic(8)){												//Pauses but returns 1 immediately that a character is received
keypress = Char_from_PC_Basic();												//Skip if no character has been received 
break;}}																//Exit while loop when character has been read
if((keypress == '\r') || (keypress == '\n')){							//Detect \r\n, \r or \n and converts to \r
if (isCharavailable_Basic(1)){temp = Char_from_PC_Basic();}
keypress = '\r';}
return keypress;}


  
  /***********************************************************************************************************************************************************************************/
void Hex_and_Int_to_PC_Basic (char radix, int number)
{ int i = 0;
 char sign = '+';
  unsigned char s[12];
unsigned int num;
  
if ((number < 0) && (radix == 10)){sign = '-'; number *= -1;}
    num = (unsigned)number;  
   
   do  { s[i] = num % radix;// + '0';
if (s[i] < 10)s[i] += '0'; else s[i] += '7';
   i += 1;  }
  while ((num = num / radix) > 0);
  s[i] = '\0';
  if ((sign == '-') && (radix == 10)){Char_to_PC_Basic('-');}
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');}



/**********************************************************************************************************************************************************************************/
void Long_Hex_and_Int_to_PC_Basic (char radix, long number)
{ int i = 0;
 char sign = '+';
  unsigned char s[12];
unsigned long num;
  
if ((number < 0) && (radix == 10)){sign = '-'; number *= -1;}
    num = (unsigned long)number;  
   
   do  { s[i] = num % radix;// + '0';
if (s[i] < 10)s[i] += '0'; else s[i] += '7';
   i += 1;  }
  while ((num = num / radix) > 0);
  s[i] = '\0';
  if ((sign == '-') && (radix == 10)){Char_to_PC_Basic('-');}
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');}
  


/**********************************************************************************************************************************************************************************/
