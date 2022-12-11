

/*Proj_8E_fp_nos_from_IO
**************************************************************/

/*A look at getting floating point numbers from the user switches.

Repeat of 5C but uses IO data entry
Calculates power until FPN limit is reached

IT INTRODUCES


1.     Subroutine "scroll_display_zero()".
              This is used to acquire the next digit (i.e. 0-9, d.p. - or E)

2.     Subroutine "shift_display_left()".
              This is used to enter the latest digit and block any subsequent digits which may now be 
              illegal (i.e. only one decimal point is allowed). 
  
3.     Subroutine FPN_as_string()
              Acquires a string such as 1234.567e8 and
              returns the number as a string and exponent as an integer

4.     Subroutine fpn_from_IO()
              Converts the string to a real number and then multiplies it by the exponent
              to obtain a floating point number (FPN).
               
*/



#include "FPN_IO_to_display_header.h"


int main (void){
  float x1;
  float power;
 
  setup_HW_Arduino_IO;

{Serial.write("Press: sw_1 to populate digit_0, sw2 to shift the display left\r\n\
sw_3 to enter the number and sw1 to clear the display.\r\n\
Note: display flashes to indicate number has been entered.\r\n");}
  
 x1 = fpn_from_IO();
 if(x1 > 0.0)power = 1.2; else power = 3.0;
 
while(1){
Sc_Num_to_PC(x1,1,6 ,'\r');
FPN_to_display(x1);

while(switch_1_down);

x1 = pow(x1, power); }                                //Do some arithmetic

while(switch_1_up);
SW_reset;}



/********************************************************************************/
float fpn_from_IO()
{
  char FPN_string[15];
  float num = 0;
  char sign = '+';
  char exp_bkp;
  char expnt;
  long Denominator;
  
  Denominator = 1;
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
  
  Converts FP string to a long number plus denominator and exponent
  */
  
  sei();
expnt = FPN_as_string(FPN_string);
exp_bkp = expnt;


if (FPN_string[0]== '-')
{for (int m = 0; m <= 13; m++)
FPN_string[m] =  FPN_string[m + 1];
sign = '-';}
num = atof(FPN_string);

if (exp_bkp > 0)
{while (expnt > 0){num = num * 10.0; expnt -=1; }}

if (exp_bkp < 0)
{while (expnt < 0){num = num / 10.0; expnt +=1; }}

if (sign == '-') num = num * (-1);
disable_pci_on_sw3;
return num;}



/*************************************************************************/
int FPN_as_string(char * Real_num_string){              //Returns the exponent

char keypress = 0;
int dig_counter = 1;
char expnt;
char keypress_E = 0;
char * expnt_add;                                         

Data_Entry_complete = 0;
digit_entry = 0;
  
while(1){                                               //Data entry loop
while (!(digit_entry));                                 //Wait here while each digit is entered
dig_counter += 1;
digit_entry = 0;
if (Data_Entry_complete)break;                          //Leave loop when data entry is complete
*(Real_num_string++) = digits[1]; _delay_us(1);         //Increment string adddress after saving digit 
if ((digits[1] == 'e') || (digits[1] == 'e'))
{expnt_add = Real_num_string;                           //Save address of the exponential string 
*(Real_num_string - 1) = '\r';                          //Terminate real number string
keypress_E = 1;}}
*(Real_num_string++) = digits[0];                        //Save final digit
*Real_num_string = '\r';                                 //Terminate string with cr.

if(keypress_E)
return atoi(expnt_add);
else return 0;}






/*************************************************************************/
ISR(PCINT0_vect){
  if(switch_3_up)return;                                 //Ignore switch release
  I2C_Tx_any_segment_clear_all();
  Timer_T0_10mS_delay_x_m(25);                           //Flash display
  I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;digit_entry = 1;}



/*************************************************************************/
ISR(PCINT2_vect){
  if((switch_1_up) && (switch_2_up))return;
  while(switch_1_down){scroll_display_zero();
  Timer_T0_10mS_delay_x_m(20);}
  if(switch_2_down)shift_display_left();
  Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw2;}



/*************************************************************************/
void shift_display_left(void){
  scroll_control &= (~(0x14));                        //negative sign & first char disabled

  switch(digits[0]){
    case '-': break;
    case '.': scroll_control &= (~(0x3)); break;      //exponent & dp disabled
    case 'e': scroll_control &= (~(0xB));             //Set RHS and disable d.p.
    scroll_control |= 0x04; break;                    //Enable neg sign
    default:  if(scroll_control & 8)                  //If LHS but not RHS
  scroll_control |= 1;break;}                         //enable exponent

  shift_digits_left; digits[0] = '0';
  I2C_Tx_8_byte_array(digits);
digit_entry = 1;}



/*************************************************************************/
void scroll_display_zero(void){                       //display scrolls 0 to 9 then minus symbol d.p. E and back to 0
  switch (digits[0]){
    case '9':
    switch(scroll_control){
      case 0b11110: digits[0] = '-'; break;           //Waiting for first character: digits[0] = '\0'
      case 0b01010: digits[0] = '.'; break;           //Waiting for second character: negative number digits[0] = '-'
      case 0b01000: digits[0] = '0'; break;           //LHS waiting for first digit (0 to 9)
      case 0b01011: digits[0] = '.'; break;           //digits[0] = 0 to 9: can receive d.p. e or additional digit
      case 0b01001: digits[0] = 'e'; break;           //Real number: can only receive e or additional digits
      case 0b00000: digits[0] = '0'; break;           //RHS: Can only receive digits
    case 0b00100: digits[0] = '-'; break;} break;     //RHS: can receive a - or a digit
    

    case '-':
    switch(scroll_control){
      case 0b11110: digits[0] = '.'; break;           //Waiting for first character: digits[0] = '\0'
    case 0b00100: digits[0] = '0'; break;} break;     //RHS: can receive a - or a digit

    case '.':
    switch(scroll_control){
      case 0b11110: digits[0] = '0'; break;           //Waiting for first character: digits[0] = '\0'
      case 0b01010: digits[0] = '0'; break;           //Waiting for second character: negative number digits[0] = '-'
    case 0b01011: digits[0] = 'e'; break;} break;     //digits[0] = 0 to 9: can receive d.p. e or additional digit
    

    case 'e':
    switch(scroll_control){
      case 0b01011: digits[0] = '0'; break;           //digits[0] = 0 to 9: can receive d.p. e or additional digit
    case 0b01001: digits[0] = '0'; break;} break;     //Real number: can only receive e or additional digits

  default: digits[0] += 1; break;}

I2C_Tx_8_byte_array(digits);}







/*******************************************************************************/
