
/*
Program synthesizes a pulse waveform on the PC screen and also does some floating point arithmetic.
Floating point results of zero or infinity cause the program to crash and trigger a WDTout with interrupt.
Switch presses are used to start the waveform adjust its parameters and trigger the FP arithmetic.
A SW_reset is triggered after 25 uninterrupted waveform cycles.

Enter duty cycle x 10 (i.e. 1 to 9)
Enter the number of time slots or harmonics as ~5 to 256
Use switch presses in place of the return key press


See https://en.wikipedia.org/wiki/Pulse_wave for details of the pulse train
*/


#include "8B_header_file_1.h"
#include "8B_header_file_2.h"

#define Buff_Length  20

float Num_1, Num_2;
char digits[12];                                                       //Array used to drive the display

int main (void){

float FPN;
float pi = 3.14159;
int print_spaces;
float Time;                                                            //< 0 Time >= 1 (Assumes a waveformperion T of unit
float amplitude;                                                       //Synthesized amplitude at and any time
float duty_cycle;
char num_as_string[Buff_Length + 2];                                                //Array used to data entry
float pulse_amplitude = 100.0;                                         //Arbitrary values chosen 
int print_offset = 25;                                                 //to fill screen

int num_time_slots;                                                    //45
int num_harmonics;                                                     //30
int counter;                                                           //Counts the number of periods printed out

setup_HW_with_reset_analysis;

set_up_PCI;

switch (reset_status)                                                    //Check each time a reset occurs
{case 1: User_prompt_A;                                                  //POR
SW_reset;break;
case 4: Timer_T1_sub_with_interrupt(T1_delay_250ms);                    //Flagged WDTout. Restart with new waveform
for(int p = 0; p <10; p++)newline_A; break;

case 5:Serial.write
("\r\n\r\nNumerical result out of bounds.\r\n");                          //WDTout with interrupt
case 2:                                                                  //SW_reset
case 3:                                                                  //Post prtD
 eeprom_write_byte((uint8_t*)(0x0),1);                                   //1 to 9 gives mark space ratio
 eeprom_write_byte((uint8_t*)(0x1),45);                                  //Waveform period
 eeprom_write_byte((uint8_t*)(0x2),0); 
 eeprom_write_byte((uint8_t*)(0x3),30);                                  //Number of harmonics
 eeprom_write_byte((uint8_t*)(0x4),0);  
 
Serial.write("\r\nEnter positive scientific number \
& terminate with Return key.\r\n");

setup_watchdog;

Num_1 = Sc_Num_from_PC_A( num_as_string, Buff_Length );
One_25ms_WDT_with_interrupt;

FPN_to_String(Num_1, 1, 3, ' ', digits);
reverse(digits);
for(int m = 0; m < 11; m++)digits[m] = digits[m+1];
I2C_Tx_8_byte_array(digits);

float_to_EEPROM(Num_1, 0x5);


Serial.write("Press SW2 or 3 to start\r\n");

while(1)
{if((switch_2_down) || (switch_3_down))break; else wdr();}              //Press switch 2 or 3 to start
enable_pci_on_sw1;break;}                                               

duty_cycle = (float)(eeprom_read_byte((uint8_t*)(0x0)))/10.0;           //Read EEPROM for waveform parameters
num_time_slots = (eeprom_read_byte((uint8_t*)(0x2)) << 8) +
eeprom_read_byte((uint8_t*)(0x1));
num_harmonics = (eeprom_read_byte((uint8_t*)(0x4)) << 8) +
eeprom_read_byte((uint8_t*)(0x3));

for(int n = 0; n <= 100; n++){if (n<40)Serial.write (' ');              //Print plot axis
else Serial.write('_');}
newline_A;

counter = 0;
while(counter < 25) {                                                   //Reset after printing out 25 identical pulse waveforms

for (int n = 0; n < num_time_slots ; n++)                               //Calculate amplitude for each time slot
{
    Time = (float)n/(float)num_time_slots;                                 //0.0 allowed because cos 0 equals unity

amplitude = 0.0;                                                        //Zero allowed for addition (but not multiplication)
for (int p = 1; p <= num_harmonics; p += 1){                            //Sum amplitude of each harmonic

wdr();
amplitude += sin(duty_cycle * pi * (float) p) *
cos(2.0 * pi * Time * (float) p) / (float) p; }                         //Formula given in Wikipedia

amplitude *= 2.0 * pulse_amplitude / pi;                                //Scale result as in Wikipedia

//if(n == 0){test_1 = Sc_Num_to_PC_A(float_from_EEPROM(0x5), 1, 2, ' ');
//print_offset -= (test_1*2 + test_1/4);}
//else print_offset = 25;

print_spaces = print_offset + 
(int)(pulse_amplitude * duty_cycle)  + (int)(amplitude);                //Add in DC term + arbitrary offset to center waveform on screen
for (int n = 0; n < print_spaces; n++){Serial.write(' ');}
Serial.write('|');_delay_ms(20);

newline_A; wdr();

wdr();}
counter += 1;}

Serial.write("\r\nTimed out: Reset initiated\r\n");
setup_watchdog; SW_reset;}


                                                       

/*******************************************************************************************************************/
ISR(PCINT2_vect){
  int data;
  if (switch_1_up)return;
  sei();
  disable_pci_on_sw1;

if((switch_2_down) && (switch_3_down)){                                 //Do some arithmetic
Num_1 = float_from_EEPROM(0x5);
Num_2 = pow(Num_1, 1.2);
if(Num_2 == Num_1)while(1);                                             //Zero or infinity: Force timeout

FPN_to_String(Num_2, 1, 2, ' ', digits);
reverse(digits);
for(int m = 0; m < 11; m++)digits[m] = digits[m+1];
I2C_Tx_8_byte_array(digits);

float_to_EEPROM (Num_2, 0x5);

Timer_T1_sub_with_interrupt(T1_delay_250ms);
return;}

data = PCI_triggers_data_from_PC(digits);
if((!(data - '0')) || (!(data))){Timer_T1_sub_with_interrupt(T1_delay_250ms);return;} 

if((switch_2_up) && (switch_3_up)){                                     //Set duty cycle
if(data > 9)data = data%10 + 1;
eeprom_write_byte((uint8_t*)(0x0),data);}
    
 if((switch_2_down) && (switch_3_up))                                   //Set period
 {eeprom_write_byte((uint8_t*)(0x1),data);
 eeprom_write_byte((uint8_t*)(0x2), (data >> 8));}

if((switch_3_down) && (switch_2_up))                                    //Set number of harmonics
{ eeprom_write_byte((uint8_t*)(0x3),data); 
 eeprom_write_byte((uint8_t*)(0x4), (data >> 8));}


Signal_flagged_WDTout;                                                  //Return after requesting new waveform
setup_watchdog; SW_reset;}
  



/*******************************************************************************************************************/
ISR(TIMER1_OVF_vect) {TIMSK1 &= (~(1 << TOIE1)); enable_pci_on_sw1;}



ISR (WDT_vect){
  Signal_WDTout_with_interrupt;}



/*******************************************************************************************************************/
void float_to_EEPROM(float num, int address){
char * Char_ptr_local;

Char_ptr_local = (char*)&num;
for (int m = 0; m <= 3; m++){
eeprom_write_byte((uint8_t*)(address++), *(Char_ptr_local++));}}



/*******************************************************************************************************************/
float float_from_EEPROM(int address){
float num;
float * Flt_ptr_local;
char * Char_ptr_local;

Flt_ptr_local = &num;
Char_ptr_local = (char*)&num;
 
for (int m = 0; m <= 3; m++){
*(Char_ptr_local++) = eeprom_read_byte((uint8_t*)(address++));}
num =  * Flt_ptr_local;
return num;}



/*******************************************************************************************************************/
int PCI_triggers_data_from_PC(char * num_as_string)  
{int m= 0;
while(1){if (Serial.available()) {num_as_string[m] = Serial.read(); 
if(decimal_digit_A(num_as_string[m]))m += 1;} else break;}
num_as_string[m] = '\0';
if (!(m))return '0';
return atoi(num_as_string);}






/*******************************************************************************************************************/
