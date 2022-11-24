


/*
Subroutines required by projects FPN/INT_from IO to read the user switch presses
*/



#define pause_PCI_and_Send_int_num_string \
S_reg_bkp = SREG; sei(); pause_PCI; Int_num_string_to_display; SREG = S_reg_bkp; reinstate_PCI;

#define pause_PCI_and_Send_float_num_string \
S_reg_bkp = SREG; sei(); pause_PCI; Float_num_string_to_display; SREG = S_reg_bkp; reinstate_PCI;




/******************************************************************************************************************************************/
void shift_float_display_left(void){

if((byte)display_buffer[0] & 0x80)dp_control = 1;                       //No more decimal points allowed
if (display_buffer[0] == 'e')
{exp_control = 1; neg_sign = 0;}                                       	//Negative exponent allowed
else neg_sign = 1;                                                      //FP number can have one negative sign 

scroll_control = (exp_control << 2) | (dp_control << 1) | neg_sign;   

shift_digits_left; 
display_buffer[0] = '0'; 
pause_PCI_and_Send_float_num_string;
digit_entry = 1;}





/******************************************************************************************************************************************/
void shift_int_display_left(void){

shift_digits_left; 
display_buffer[0] = '0'; 
pause_PCI_and_Send_int_num_string;
digit_entry = 1;}




/********************************************************************************************************/
void scroll_float_display_zero(void){
unsigned char data_zero;

disable_PCI_on_sw3;

data_zero = display_buffer[0];

if (switch_3_up){display_buffer[0] &= (~(0x80));             //Decimal point dissabled

switch (display_buffer[0]){
case '9':
switch(scroll_control){ 
case 0: display_buffer[0] = '-'; break;                    //Initial state scroll 0-9 -ve 0...... dp is enabled
case 1: display_buffer[0] = '0'; break;                    //First char entered -ve sign dissabled  dp enabled
case 3: display_buffer[0] = 'e'; break;                    //Decimal point entered.  Dissable DP. Enable E.
case 6: display_buffer[0] = '-'; break;                    //E entered. Enable -ve sign
case 7: display_buffer[0] = '0'; break;}break;              //Dissable -ve sign

case '-':
switch(scroll_control){
case 0: display_buffer[0] = '0'; break;                    //Initial state
case 6: display_buffer[0] = '0'; break;}break;

case 'e':
switch(scroll_control){
case 0: display_buffer[0] = '0'; break;                       //Initial state
case 1: display_buffer[0] = '0'; break;    
case 3: display_buffer[0] = '0'; break;}break;

default: display_buffer[0] += 1; break;}}

if((switch_3_down) && (!(dp_control)))
{display_buffer[0] |= 0x80;                                   //Decimal point enabled 

switch ((byte)display_buffer[0] & (~(0x80))){
case '9': 
display_buffer[0] = ('0' | 0x80);break;
default:  display_buffer[0] += 1;break;}}

pause_PCI_and_Send_float_num_string;}





/********************************************************************************************************/
void scroll_int_display_zero(void){	
unsigned char data_zero;

switch (display_buffer[0]){
case '9': if(!(display_buffer[1])) 
display_buffer[0] = '-'; 
else display_buffer[0] = '0'; break;
case '-': display_buffer[0] = '0'; break;
default: display_buffer[0] += 1;break;}

pause_PCI_and_Send_int_num_string;
return;}



