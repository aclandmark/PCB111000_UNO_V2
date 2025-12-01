

void copy_cmd_to_page_buffer(void);
void get_next_hex_cmd(void);

void binUnwantedChars_dot (void);
void Text_to_EEPROM(int*, char);
char Text_from_EEPROM(int*);
void newline(void);
void sendSpace(void);
void sendString(const char *);

void Num_to_PC(char, long);


/**********************************************************************************************************/
void new_record(void){int Local_r_pointer;								//Start of new record from the hex file

while (r_pointer == w_pointer);											//Wait for new entry in array "store"
Local_r_pointer = r_pointer;
record_length =  store[Local_r_pointer]; 								//Obtain the length of the new record
inc_r_pointer;  														//Increment the read pointer
Count_down = record_length/2; 											//Initialised to record lenghth (in commands) 
if (record_length_old < 0x10)short_record=1;							//Check for "record length" (<8 commands)
record_length_old = record_length;										//Save record length

while (r_pointer == w_pointer);											//Wait for next entry in array "store"
Local_r_pointer = r_pointer;
Hex_address  =  (store[Local_r_pointer]); 								//Get the address of the first command in the new record
inc_r_pointer;  														//Increment the read pointer
Hex_address  = Hex_address/2;											//Convert address from byte to word value
prog_led_control++;

if (Hex_address == HW_address)orphan = 0;								//New record follows on immediately from the old
else{if (Hex_address == 0);											//Start of hex file: address is zero
		else{if ((Hex_address & PAmask) > (page_address + PageSZ))		//Long jump: At least one page is unused
				{section_break=1;page_break=0;}
			if((Hex_address & PAmask) == (page_address + PageSZ))		//Page jump: Jump to next page without filling current one
				{section_break=0;page_break=1; }
			if ((Hex_address & PAmask) == page_address)				//Short jump: Jump within page
				{section_break=0;page_break=0; orphan = 0;}}}

if(prog_led_control & 0b00001000) {LEDs_on;}	
else {LEDs_off;}}  



/**********************************************************************************************************/
void start_new_code_block(void){
HW_address = Hex_address;												//Initialise HW_address
page_address = (Hex_address & PAmask);									//Obtain page address
write_address = Hex_address - page_address;								//Initialise write_address: zero to Page size -1
page_offset = write_address;											//"page_offset" is the initial value of "write_address"											
line_offset = Hex_address & 0x0007;										//Not all lines start with addresses that are multiples of 8
space_on_page = (PageSZ - page_offset);}								//Initialise "space_on_page"



/**********************************************************************************************************/
void Program_record(void){

while(Count_down){ 														//Initially contains the number of commands in the line
Count_down--;															//Decrement "count_down" each time a command is written to the page buffer
Flash_flag = 1;  														//Indicates that page buffer containes commands that will need burning to flash 
copy_cmd_to_page_buffer();  												 
write_address++;
HW_address++;
space_on_page--;
prog_counter++;       
if (write_address == PageSZ){											//If page_buffrer is now full:
Read_write_mem('W', page_address, 0x0);									//Burn contents of page buffer to flash
Flash_flag = 0;															//Buffer now contains no data to burn to flash
write_address = 0;														//"while loop" continues if there is a line offset
space_on_page = PageSZ;
page_offset = line_offset;
if (line_offset) orphan = 1;}}}										//One or more commands in current record will be on next page



/**********************************************************************************************************/
void copy_cmd_to_page_buffer(void){
get_next_hex_cmd();
Load_page('L', write_address, Hex_cmd & 0x00FF); 
Load_page('H', write_address, (Hex_cmd >> 8));}



/**********************************************************************************************************/
void get_next_hex_cmd(void){int Local_r_pointer;
while (r_pointer == w_pointer);
Local_r_pointer = r_pointer;
Hex_cmd = store[Local_r_pointer];
inc_r_pointer;}



/**********************************************************************************************************/
void write_page_SUB(signed int page_address){
Read_write_mem('W',  page_address, 0x0);
Flash_flag = 0;}



/***********************************************************/
char receive_byte_with_Ack(void){
char byte;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);						//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));										//Wait for interrupt
byte = TWDR;
return byte;}



/***********************************************************/
char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);										//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));										//Wait for interrupt
byte = TWDR;
return byte;}



/*********************************************************************/
void binUnwantedChars_dot (void){char bin_char;
while(1){if (isCharavailable(5)==1)
{bin_char = receiveChar();sendChar('.');}else break;}newline();}




/*****************************************************************************************************/




