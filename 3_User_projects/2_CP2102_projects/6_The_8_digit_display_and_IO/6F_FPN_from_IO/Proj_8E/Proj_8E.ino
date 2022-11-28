

/*Proj_8E_fp_nos_from_IO
**************************************************************/

/*A look at getting floating point numbers from the user switches.


IT INTRODUCES


1.  Project subroutine "scroll_display_zero()".
  This is used to acquire the next digit (i.e. 0-9, d.p. - or E)

2.  Project subroutine "shift_display_left()".
  This is used to enter the latest digit and note any subsequent digits which may now be 
  illegal (i.e. only one decimal point is allowed). 
  
3.  Project subroutine "Double_from_IO()".
  This is where program execution pauses while the user is entering data at the keyboard. 
  A return key press causes program execution to  leave this subroutine.
  
4.  Project subroutine "acquire_fpn()" the subroutine used to initiate the acquisition of a fpn.

*/



#include "Proj_8E_header_file.h"


volatile char Data_Entry_complete, digit_entry;
volatile char scroll_control;
char digits[8];


int main (void){
  long x1;
  char expnt;
  long Denominator;

  setup_HW_Arduino_IO;

//if((!watch_dog_reset))
{
Serial.write("Press: sw_1 to populate digit_0, sw2 to shift the display left\r\n\
sw_3 to enter the number and sw1 to clear the display.\r\n\
Note: display flashes to indicate number has been entered.\r\n");}

  watch_dog_reset = 0;

 
  x1 = fpn_from_IO(&expnt, &Denominator);
  Serial.write("\r\n");
 Serial.print(x1); 
 Serial.write("\r\n");
 Serial.print(Denominator); 
  while(switch_1_up);
SW_reset;}



/********************************************************************************/
long fpn_from_IO(char *expnt, long *Denominator)
{
  char FPN_string[15];
  long num_1=0, num_2 = 0;
  char sign = '+';

  
  *Denominator = 1;
  
  for(int m = 0; m <= 14; m++)FPN_string[m] = 0;
  
  set_up_pci;
  enable_pci;
  
  Init_display_for_pci_data_entry;
  scroll_control = 0b00011110;
  /*
  bit 0: exponent disabled
  bit 1: decimal point enabled
  bit 2: negative sign enabled
  bit 3: LHS of exponent
  bit 4: Waiting for first character
  */
  
  sei();
*expnt = FPN_as_string(FPN_string);


Serial.write("\r\n");
Serial.print((int)*expnt);
Serial.write("\r\n");
for(int m = 0; m <= 14; m++)if(FPN_string[m]) Serial.write(FPN_string[m]);//

if (FPN_string[0]== '-'){for (int m = 0; m <= 13; m++)FPN_string[m] =  FPN_string[m + 1];
sign = '-';}
num_1 = atol(FPN_string);
num_2 = num_1;
while(num_2){(*expnt)++; *Denominator *=10; num_2 /= 10; }

if (sign == '-') num_1 = num_1 * (-1);
disable_pci_on_sw3;
return num_1;}



/*************************************************************************/
int FPN_as_string(char * FPN_string){
  char keypress = 0;
  int dig_counter = 1;
  char expnt, test;
    
    char * string_add;
    string_add = FPN_string;
    
  Data_Entry_complete = 0;
  digit_entry = 0;
  
 while(1){
while (!(digit_entry));
dig_counter += 1;
digit_entry = 0;
if (Data_Entry_complete)break;
*(FPN_string++) = digits[1]; _delay_us(1);  
}
*(FPN_string++) = digits[0]; 
*FPN_string = '\r';

while(dig_counter){if (*(--FPN_string) != 'e')dig_counter -= 1; else break;} 
//if (*(FPN_string+1) == 'e')(*(FPN_string+1) == 0);
if(!(dig_counter))expnt = 0;
else
{expnt = atoi ((++FPN_string));

Serial.write("\r\n");
test = *string_add;
Serial.write(test);


/*{int m,p;
for (m = 0; m <=14; m++)if(*(string_add + m) == '.')break;
if(*(string_add + m) == '.'){for (int p = m; p <= 14; p++)(*(string_add + p)) = (*(string_add + p + 1));}}*/


return expnt;}

}




/*************************************************************************/
ISR(PCINT0_vect){
  if(switch_3_up)return;                        //Ignore switch release
  I2C_Tx_any_segment_clear_all();
  Timer_T0_10mS_delay_x_m(25);                    //Flash display
  I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;digit_entry = 1;}



/*************************************************************************/
ISR(PCINT2_vect){
  if((switch_1_up) && (switch_2_up))return;
  while(switch_1_down){scroll_display_zero();Timer_T0_10mS_delay_x_m(20);}
  if(switch_2_down)shift_display_left();
  Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw2;}



/*************************************************************************/
void shift_display_left(void){
  scroll_control &= (~(0x14));                    //negative sign & first char disabled

  switch(digits[0]){
    case '-': break;
    case '.': scroll_control &= (~(0x3)); break;          //exponent & dp disabled
    case 'e': scroll_control &= (~(0xB));             //Set RHS and disable d.p.
    scroll_control |= 0x04; break;                  //Enable neg sign
    default:  if(scroll_control & 8)                //If LHS but not RHS
  scroll_control |= 1;break;}                     //enable exponent

  shift_digits_left; digits[0] = '0';
  I2C_Tx_8_byte_array(digits);
digit_entry = 1;}



/*************************************************************************/
void scroll_display_zero(void){                     //display scrolls 0 to 9 then minus symbol d.p. E and back to 0
  switch (digits[0]){
    case '9':
    switch(scroll_control){
      case 0b11110: digits[0] = '-'; break;           //Waiting for first character: digits[0] = '\0'
      case 0b01010: digits[0] = '.'; break;           //Waiting for second character: negative number digits[0] = '-'
      case 0b01000: digits[0] = '0'; break;           //LHS waiting for first digit (0 to 9)
      case 0b01011: digits[0] = '.'; break;           //digits[0] = 0 to 9: can receive d.p. e or additional digit
      case 0b01001: digits[0] = 'e'; break;           //Real number: can only receive e or additional digits
      case 0b00000: digits[0] = '0'; break;           //RHS: Can only receive digits
    case 0b00100: digits[0] = '-'; break;} break;         //RHS: can receive a - or a digit
    

    case '-':
    switch(scroll_control){
      case 0b11110: digits[0] = '.'; break;           //Waiting for first character: digits[0] = '\0'
    case 0b00100: digits[0] = '0'; break;} break;         //RHS: can receive a - or a digit

    case '.':
    switch(scroll_control){
      case 0b11110: digits[0] = '0'; break;           //Waiting for first character: digits[0] = '\0'
      case 0b01010: digits[0] = '0'; break;           //Waiting for second character: negative number digits[0] = '-'
    case 0b01011: digits[0] = 'e'; break;} break;         //digits[0] = 0 to 9: can receive d.p. e or additional digit
    

    case 'e':
    switch(scroll_control){
      case 0b01011: digits[0] = '0'; break;           //digits[0] = 0 to 9: can receive d.p. e or additional digit
    case 0b01001: digits[0] = '0'; break;} break;         //Real number: can only receive e or additional digits

  default: digits[0] += 1; break;}

I2C_Tx_8_byte_array(digits);}
