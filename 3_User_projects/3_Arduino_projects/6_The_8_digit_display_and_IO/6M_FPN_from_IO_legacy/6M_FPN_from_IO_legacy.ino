

/*Proj_8E_fp_nos_from_IO  OLD method suiable for PCB111000_original with winAVR
**********************************************************************************/

/*A look at getting floating point numbers from the user switches.

Repeat of 5C but uses IO data entry
Calculates power until FPN limit is reached

IT INTRODUCES


1.  Project subroutine "scroll_display_zero()".
  This is used to acquire the next digit (i.e. 0-9, d.p. - or E)

2.  Project subroutine "shift_display_left()".
  This is used to enter the latest digit and note any subsequent digits which may now be 
  illegal (i.e. only one decimal point is allowed). 
  
3.  Project subroutine "fpn_from_IO()" the subroutine used to initiate the acquisition of a fpn.

3.  Project subroutine "FPN_as_string()".
  This is where program execution pauses while the user is entering data at the keyboard. 

1.     Subroutine FPN_as_string()
              Acquires a string such as 1234.567e8 and
              Converts it to 12345678 with an exponent of 4 

2.     Subroutine fpn_from_IO()
              Generates the string 12345678 known as the significand which is converted to a long number and
              a denominator of 100000000 is generated 

3.     Subroutine fpn_from_IO()
              Converts the significant to a long number and
              Generates a denominator 100000000 in the case. It also
              Adjusts the exponent so that 1234.5678e8 can represented as 0.12345678e12

4.     Subroutine Significand_to_FPN()
            Generates the FPN 12345678/100000000 which it stores in float format.
            It then multiplies this number by 10^12 and returns it in a forn suitable for use by 
            the standard c and arduino libraries


  
*/



#include "FPN_IO_to_display_header.h"


int main (void){
  float x1;
  float power;
  char expnt;
  long Denominator;
  long Significand;

  setup_HW_Arduino_IO;

if(!(watch_dog_reset))

{Serial.write("Press: sw_1 to populate digit_0, sw2 to shift the display left\r\n\
sw_3 to enter the number and sw1 to clear the display.\r\n\
Note: display flashes to indicate number has been entered.\r\n");}

else {Serial.write("\r\nAgain\r\n"); watch_dog_reset = 0;}

 
 Significand = fpn_from_IO_Legacy(&expnt, &Denominator);
 x1 = Significand_to_FPN((float)Significand, Denominator, expnt);
 if(x1 > 0.0)power = 1.2; else power = 3.0;
 
while(1){
Sc_Num_to_PC_A(x1,1,6 ,'\r');
Significand = FPN_to_Significand(x1, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;

while(switch_1_down);

x1 = pow(x1, power); }                                //Do some arithmetic

while(switch_1_up);
SW_reset;}



/********************************************************************************/
long fpn_from_IO_Legacy(char *expnt, long *Denominator)
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
  
  Converts FP string to a long number plus denominator and exponent
  */
  
  sei();
*expnt = FPN_as_string_Legacy(FPN_string);

if (FPN_string[0]== '-')
{for (int m = 0; m <= 13; m++)
FPN_string[m] =  FPN_string[m + 1];
sign = '-';}
num_1 = atol(FPN_string);
num_2 = num_1;
while(num_2){(*expnt)++; *Denominator *=10; 
num_2 /= 10; }

if (sign == '-') num_1 = num_1 * (-1);
disable_pci_on_sw3;
return num_1;}



/*************************************************************************/
int FPN_as_string_Legacy(char * FPN_string){                       //Returns the exponent

char keypress = 0;
int dig_counter = 1;
char expnt;

char decimal_place = 0;
char decimal_place_counter = 0;
char keypress_E = 0;
char * string_add;                                         //Saves address of FPN_string

string_add = FPN_string;
Data_Entry_complete = 0;
digit_entry = 0;
  
while(1){                                                   //Data entry loop
while (!(digit_entry));                                    //Wait here while each digit is entered
dig_counter += 1;
digit_entry = 0;
if (Data_Entry_complete)break;                              //Leave loop when data entry is complete
*(FPN_string++) = digits[1]; _delay_us(1);                  //Increment string adddress after saving digit 

if (digits[1] == '.'){decimal_place = 1;continue;}          //Skip bact to top of loop if decimal place is entered         
if (digits[1] == 'e'){keypress_E = 1; continue;}
if((decimal_place) && (!(keypress_E)))
decimal_place_counter += 1;}                                //Only count decimal places untill E is detected

*(FPN_string++) = digits[0];                                //Save final digit
*FPN_string = '\r';                                         //Terminate string with cr.

if((decimal_place_counter)&& (!keypress_E))
decimal_place_counter += 1;                                 //Increment decimal_place counter if there is no E

while(dig_counter){if (*(--FPN_string) != 'e')              //Get the location of E
dig_counter -= 1; else break;} 
if (*(FPN_string) == 'e')(*(FPN_string) = 0);               //Replace the E with the null character
if(!(dig_counter))expnt = 0;
else expnt = atoi ((++FPN_string));                         //Calculate the value of the exponent

{int m,p;
for (m = 0; m <=14; m++)
if(*(string_add + m) == '.')break;                          //Locate the decimal place
if(*(string_add + m) == '.')
{for (int p = m; p <= 14; p++)
(*(string_add + p)) = (*(string_add + p + 1));}}            //Sfift digits to remove the decimal point

expnt -= decimal_place_counter;                             //Adjust the exponent to account for the decimal places

return expnt;}






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







/*******************************************************************************/
