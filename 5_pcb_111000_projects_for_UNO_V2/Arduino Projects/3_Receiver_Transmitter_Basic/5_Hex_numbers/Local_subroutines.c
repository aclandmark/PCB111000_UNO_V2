
char wait_for_return_key_Basic(void);
long I2C_displayToNum(void);
void I2C_Tx_8_byte_array(char*);
char decimal_digit_Basic (char data);
char waitforkeypress_Basic (void);
void Char_to_PC_Basic(char);
void Num_string_from_KBD_Local(char *);
long Askii_to_binary_Local(char, char *);




/*****************************************************************************************************************/
long Int_from_PC_local(char radix, char * array_ptr){
Num_string_from_KBD_Local(array_ptr);
  return Askii_to_binary_Local(radix, array_ptr);}




/***********************Subroutines*********************************/
void Num_string_from_KBD_Local(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    Char_to_PC_Basic(keypress);
    array_ptr += 1;
  }
  *array_ptr = '\0';}




/********************************************************************************************************/
long Askii_to_binary_Local(char radix, char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
       switch(*(array_ptr + m)){
case '0': case '1': case '2': case '3': case '4': case '5': case '6': 
case '7': case '8': case '9':  num = num * radix + (*(array_ptr + m++))  - '0'; break;
case 'A': case 'B': case 'C': case 'D': case 'E':
case 'F': num = num * radix + (*(array_ptr + m++))  - '0' - 7; break;
      }}}
  return num;}



/****************************************************************************************************************/
char Long_Hex_and_Int_to_PC_Basic_local (char radix, long number)
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
  Char_to_PC_Basic(' ');
  return i;}





  /***********************************************************************************************************/
