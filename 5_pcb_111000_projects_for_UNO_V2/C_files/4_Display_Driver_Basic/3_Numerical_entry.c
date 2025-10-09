

int main (void){

char   digit='0';
int digit_num=0;                 //defines number of next digit on display           
int string_counter=0;
int letter_counter=0;
const char* string_ptr = 0;   	//pointer: will be loaded with the address of a segment string 
								//(i.e. the address of string "zero", "one", "two" etc....) 

setup_HW;               


if(watch_dog_reset != 1){
print_memory_contents;
String_to_PC_Basic("\r\nSend digits?");}

else String_to_PC_Basic("\r\nAgain");

I2C_Tx_any_segment_clear_all();

digit_num = 0;                              //First digit on display

do{                                         //start of "do{}while();" loop
while(!(isCharavailable_Basic(1)))wdr(); 
digit = Char_from_PC_Basic();          		//user enters digit (0 to 9) at the PC keyboard

switch(digit){            			//The appropriate address is loaded into location 
									//"string_pointer"
case '0': string_ptr = zero; break; //The address of array zero is loaded into 
									//location "string_ptr"
case '1': string_ptr = one; break;
case '2': string_ptr = two; break;
case '3': string_ptr = three; break;
case '4': string_ptr = four; break;
case '5': string_ptr = five; break;
case '6': string_ptr = six; break;
case '7': string_ptr = seven; break;
case '8': string_ptr = eight; break;
case '9': string_ptr = nine; break;
default: continue; break;} 		//Illegal key press: Go immediately to the start of the 
								//do loop. 

								//Send the address of the required string to 
								//subroutine "display_num_string();"
display_num_string(string_ptr, digit_num);digit_num++;} 
while (digit_num < 8);   		//return to the top of the "do" loop until all digits 
								//have been illuminated

while(!(isCharavailable_Basic(1)))wdr(); Char_from_PC_Basic();
I2C_Tx_any_segment_clear_all();                //clear display and repeat
SW_reset;}
