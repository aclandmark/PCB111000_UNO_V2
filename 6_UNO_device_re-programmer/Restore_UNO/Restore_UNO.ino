

/*****ATMEGA programmer "Restore_UNO" developed to program an Atmega328 device with the UNO code or restore a UNO pcb 

*******Program the Atmega168 with "Restore_UNO.ino.eightanaloginputs.hex" using a UNO loaded with programming code "8_UNO_AVR_Programmer_V2"
This sets the fuses as follows:
                    Fuse_Ex = 0xFF;
                    Fuse_H = 0xD7;
                    Fuse_L = 0xE2;
                    Lock = 0xFF;

**** Using bread board connect the Atmega168 to a CP2102 USB module and the Atmega 328 or UNO progamming interface


At the standard user prompt "P/S   P/S   P/S........" 
press P and the user prompt changes to "p/r   p/r   p/r   p/r........"

Press -p- and send file   UNO_Arduino_bootloader.hex

******************************************************/




#include "Restore_UNO_header.h"

int main (void){ 
unsigned int target_type = 0, target_type_M;  
char keypress;
char op_code = 0;
unsigned char fuse_H = 0;


cal_factor= 0;                                                  //Set to 1 by OSC_CAL if user cal is available
setup_HW;

while(1){
do{sendString("P/S   ");} 
while((isCharavailable(255) == 0));  

switch(receiveChar()){

case 'P': case 'p': fuse_H = 0xDE;  op_code =1; break;       //Program reset vector to 0x7E00            
default:break;}if(op_code)break;}                           //Only exit loop the program target 
    
boot_target;
Atmel_powerup_and_target_detect;        

switch (target){
case 168: sendString ("\r\n168: Not supported"); wdt_enable(WDTO_1S); break;  
case 328: sendString ("\r\nAtmega 328 detected"); PageSZ = 0x40; PAmask = 0x3FC0; 
FlashSZ=0x4000;break;      
default: wdt_enable(WDTO_1S); while(1);break;}  

sendString("\r\nPress p and send file UNO_Arduino_bootloader.hex\r\n ");
while(1){

do{sendString("p/e    ");} 
while((isCharavailable(255) == 0));
op_code = receiveChar();        

switch (op_code){
case 'p':                                                       //Program target
case 'P': break;
case 'x':                                                       //Escape
case 'X': wdt_enable(WDTO_60MS); while(1);break;
default: break;} 

if ((op_code == 'P') || (op_code == 'p')) break;}            //Enter target programming mode

Initialise_variables_for_programming_flash;
//Text_Send_HexF;

while ((keypress = waitforkeypress()) != ':')                 //Ignore characters before the first ':'
{if (keypress == 'x'){wdt_enable(WDTO_60MS); while(1);}}      //X pressed to escape
UCSR0B |= (1<<RXCIE0); sei();                                 //Enable UART interrupt

(Atmel_config(Chip_erase_h, 0));                              //Only erase flash when hex file is on its way

Program_Flash();

Atmel_config(write_fuse_bits_H_h,fuse_H );                    //Write to config bytes
if(fuse_H == 0xDE)
{Atmel_config(write_extended_fuse_bits_h,0x05 );
Atmel_config(write_fuse_bits_h,0xFF );}
Atmel_config(write_lock_bits_h,0xFF );                        

sendString("\r\nAK to verify\r\n");
waitforkeypress();
Verify_Flash();  
newline();
sendString("Config bytes: Fuses extended, high, low and lock\t");

Num_to_PC(16, (byte)Atmel_config(read_extended_fuse_bits_h, 0));sendString("  ");
Num_to_PC(16, (byte)Atmel_config(read_fuse_bits_H_h,0));  sendString("  ");
Num_to_PC(16, (byte)Atmel_config(read_fuse_bits_h, 0));sendString("  ");
Num_to_PC(16, (byte)Atmel_config(read_lock_bits_h, 0));newline();

sendString("Hex_file_size:");
sendString("\r\nCommands downloaded, programmed and read back:\t");

Num_to_PC(10,cmd_counter); 
sendSpace();sendString("d'loaded:"); 
sendSpace(); 
Num_to_PC(10,prog_counter); 
sendString(" in:"); 
sendSpace(); 
Num_to_PC(10,read_ops); 
sendString(" out");
newline();

{wdt_enable(WDTO_60MS); while(1);}    
 return 1;}


/***************************************************************************************************************************************************/
ISR(USART_RX_vect){
  unsigned char Rx_Hex_char=0;
  unsigned char Rx_askii_char;
  int local_pointer;
    
Rx_askii_char = receiveChar();
switch (Rx_askii_char){
case '0':  Rx_Hex_char = 0x00; break;                         //Convert askii chars received from hex file to binary digits
case '1':  Rx_Hex_char = 0x01; break;
case '2':  Rx_Hex_char = 0x02; break;
case '3':  Rx_Hex_char = 0x03; break;
case '4':  Rx_Hex_char = 0x04; break;
case '5':  Rx_Hex_char = 0x05; break;
case '6':  Rx_Hex_char = 0x06; break;
case '7':  Rx_Hex_char = 0x07; break;
case '8':  Rx_Hex_char = 0x08; break;
case '9':  Rx_Hex_char = 0x09; break;
case 'A':  Rx_Hex_char = 0x0A; break;
case 'B':  Rx_Hex_char = 0x0B; break;
case 'C':  Rx_Hex_char = 0x0C; break;
case 'D':  Rx_Hex_char = 0x0D; break;
case 'E':  Rx_Hex_char = 0x0E; break;
case 'F':  Rx_Hex_char = 0x0F; break;
case ':':  counter = 0;  break;
default: break;}

switch (counter){
case 0x0:   break;                                        //Detect -:- at start of new line
case 0x1:   tempInt1 = Rx_Hex_char<<4;  break;            //Acquire first digit 
case 0x2:   tempInt1 += Rx_Hex_char;                      //Acquire second digit and combine with first to obtain number of commands in line
      char_count = 9 + ((tempInt1) *2);                   //Calculate line length in terms of individual characters
      local_pointer = w_pointer++;                        //Update pointer to array "store"
      store[local_pointer] = tempInt1; break;             //Save the number of commands in the line to the array  
case 0x3:   tempInt1 = Rx_Hex_char<<4;  break;            //Next 4 digits give the address of the first command in the line
case 0x4: tempInt1 += Rx_Hex_char; 
      tempInt1=tempInt1<<8; break;                        //Acquire second digit and combine it with first 
case 0x5: tempInt1 += Rx_Hex_char<<4;  break;             //Continue for third digit
case 0x6:   tempInt1 += Rx_Hex_char;                      //Acquire final digit and caculate address of next command 
      local_pointer = w_pointer++;                        //Update pointers to array "store"
      store[local_pointer] = tempInt1; break;             //Save address of next command to array "store"
case 0x7:   break;                                        //chars 7 and 8 are not used
case 0x8:   break;
default:  break;}

if ((counter > 8)&&(counter < char_count)){               //Continue to acquire, decode and store commands
if ((counter & 0x03) == 0x01){tempInt1 = Rx_Hex_char<<4;} //Note: Final two chars at the end of every line are ignored
if ((counter & 0x03) == 0x02)  {tempInt1 += Rx_Hex_char;}
if ((counter & 0x03) == 0x03)  {tempInt2 = Rx_Hex_char<<4;}
if ((counter & 0x03) == 0x0)    {tempInt2+= Rx_Hex_char; 
                tempInt2=tempInt2<<8;tempInt1+=tempInt2;
local_pointer = w_pointer++;  
store[local_pointer] = tempInt1; cmd_counter++;}}

counter++;
w_pointer = w_pointer & 0b00011111; }                       //Overwrites array after 32 entries



/***************************************************************************************************************************************************/
void Program_Flash (void){

new_record();                                           //Start reading first record which is being downloaded to array "store" 
start_new_code_block();                                 //Initialise new programming block (usually starts at address zero but not exclusivle so)
Program_record();                                       //Coppy commands from array "store" to the page_buffer                            
      
    
while(1){   
new_record();                                           //Continue reading subsequent records
if (record_length==0)break;                             //Escape when end of hex file is reached


if (Hex_address == HW_address){                         //Normal code: Address read from hex file equals HW address and lines contains 8 commands
switch(short_record){
case 0: if (space_on_page == (PageSZ - line_offset))    //If starting new page
      {page_address = (Hex_address & PAmask);}          //get new page address
      break;

case 1: start_new_code_block();                         //Short line with no break in file (often found in WinAVR hex files).
    short_record=0;break;}}
    
    
if(Hex_address != HW_address){                          //Break in file
  if (section_break){                                   //Section break: always found when two hex files are combined into one                    
    if((Flash_flag) && (!(orphan)))
    {write_page_SUB(page_address);}                     //Burn contents of the partially full page buffer to flash
    if(orphan) 
    write_page_SUB(page_address + PageSZ);}             //Burn outstanding commands to the next page in flash     
    
  if(page_break)                                        //In practice page breaks and short jumps are rarely if ever found                      
    {if((Flash_flag) && (!(orphan)))                    //Burn contents of the partially filled page buffer to flash
    {write_page_SUB(page_address);}                           
    orphan = 0;}
    
  start_new_code_block();                               //A new code block is always required where there is a break in the hex file.
  short_record=0;}
    
Program_record();}                                      //Continue filling page_buffer
    

cli();  
UCSR0B &= (~(1<<RXCIE0));                             //download complete, disable UART Rx interrupt
LEDs_off;       
while(1){if (isCharavailable(1)==1)receiveChar();
    else break;}                                      //Clear last few characters of hex file
  
if((Flash_flag) && (!(orphan)))
{write_page_SUB(page_address);}                       //Burn final contents of page_buffer to flash
if(orphan) {write_page_SUB(page_address + PageSZ);}}




/***************************************************************************************************************************************************/
void Verify_Flash (void){
signed int phys_address;                                            //Address in flash memory
signed int prog_counter_mem;                                       //Initialised with size of .hex file used for programming

phys_address = 0;  read_ops=0;     //0x3F00
prog_counter_mem = prog_counter; 

while(1){ if(!(prog_counter_mem))break;                             //print out loop starts here, exit when finished

while(1) {                                                          //Start reading the flash memory searching for the next hex command
Hex_cmd = (byte)Read_write_mem('L',phys_address, 0x0);              
Hex_cmd = (Hex_cmd<<8) + (byte)(Read_write_mem('H',phys_address, 0x0)); 
phys_address++;        
if (!(phys_address%500))sendChar ('*');
if (phys_address == FlashSZ)break;                                     //No more memory? Quit if yes
if (Hex_cmd != 0xFFFF) break;                                         //If the hex command is 0xFFFF remain in this loop otherwise exit.
LEDs_on;}

LEDs_off;
if (phys_address == FlashSZ)break;                                    //Exit when there is no more flash to read
   
{newline(); sendHex (16, (phys_address)*2);         

sendHex (16, Hex_cmd);}                                               //Print first command in askii or hex
read_ops++;                                                             //Value to be sent to PC for comparison with the hex filer size
prog_counter_mem--;                                                     //"prog_counter_mem" decrements to zero when the end of the file is reached

for(int m=0; m<15; m++){                                                //Read the next seven locations in the flash memory   
Hex_cmd = (byte)Read_write_mem('L',phys_address, 0x0);        
Hex_cmd = (Hex_cmd<<8) + (byte)(Read_write_mem('H',phys_address, 0x0)); 
phys_address++; 
prog_counter_mem--;
sendHex (16, Hex_cmd);
read_ops++;

if(phys_address==FlashSZ)break;}
//sendString("\r\n");
if (phys_address == FlashSZ)break;}

LEDs_off;
newline();newline(); }


/*********************************************************************************/





/************************************************************************************/
