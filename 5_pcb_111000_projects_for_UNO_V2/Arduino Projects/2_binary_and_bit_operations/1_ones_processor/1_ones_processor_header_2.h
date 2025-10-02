
//char User_response;


#define port_reg   digits[0]
#define cpu_reg_2   digits[1]
#define cpu_reg_1   digits[2]
#define Display_registers   I2C_Tx_BWops(digits)


/******************************************************************************************************/
#define System_response \
switch(mode){\
case 'r': String_to_PC_Basic("\r\n\r\nClear bits"); break;\
case 's': String_to_PC_Basic("\r\n\r\nSet bits"); break;\
case 'c': String_to_PC_Basic("\r\n\r\nToggle bits"); break;\
case 't': String_to_PC_Basic("\r\n\r\nTest one bit"); break;\
default:  String_to_PC_Basic("\t? ak\t");if (waitforkeypress_Basic() == 'x'){SW_reset;}\
          String_to_PC_Basic("Again!");mode = 'x';break;}\
if (mode != 'x')String_to_PC_Basic("\tStep 1: Set up CPU register");\

//default: String_to_PC_Basic("\t?");if (waitforkeypress_Basic() == 'x'){SW_reset;}  mode = 'x';break;}

/************************************************************************************************/
#define update_display \
Display_registers;\
cpu_reg_1 = 0;\
if(mode == 't'){String_to_PC_Basic("\t  ak");waitforkeypress_Basic();}\
else _delay_ms(500);\
Display_registers;\
if(cpu_reg_2){\
cpu_reg_2 = 0;\
String_to_PC_Basic("\t  ak");waitforkeypress_Basic();}\
if (mode == 't'){Display_registers;}\
for(int m = 0; m <=7; m++)reg_bkp[m] = 0;\
String_to_PC_Basic("\r\n\r\nAgain");


/**********************************************************************************************************/
#define clear_bit_1_text \
String_to_PC_Basic("\r\nStep 2: Invert CPU register");\
waitforkeypress_Basic();

#define clear_bit_2_text \
Display_registers;\
String_to_PC_Basic("\r\nStep 3  AND with PORT register");\
waitforkeypress_Basic();

#define set_bit_text \
String_to_PC_Basic("\r\nStep 2  OR with PORT register");\
waitforkeypress_Basic();

#define change_bit_text \
String_to_PC_Basic("\r\nStep 2  EOR with PORT register");\
waitforkeypress_Basic();

#define test_bit_text \
String_to_PC_Basic("\r\nStep 2  AND with PORT register");\
waitforkeypress_Basic();





/*************************************************************************/
