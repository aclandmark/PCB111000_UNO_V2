
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


#include "8C_header_file_1.h"
#include "8C_header_file_2.h"

float Num_1, Num_2;
char digits[12];                                                       //Array used to drive the display

int main (void){

float FPN;
float pi = 3.14159;
int print_spaces;
float Time;                                                            //< 0 Time >= 1 (Assumes a waveformperion T of unit
float amplitude;                                                       //Synthesized amplitude at and any time
float duty_cycle;
char num_as_string[22];                                                //Array used to data entry
float pulse_amplitude = 100.0;                                         //Arbitrary values chosen 
int print_offset = 100;                                                 //to fill screen

int num_time_slots;                                                    //45
int num_harmonics;                                                     //30
int counter;                                                           //Counts the number of periods printed out

float Harmon_1, Harmon_2, Harmon_3, Harmon_4, Harmon_5;


setup_HW_with_reset_analysis;

set_up_PCI;
setup_watchdog;

if(reset_status != 2){
eeprom_write_byte((uint8_t*)(0x0),1);                                   //1 to 9 gives mark space ratio
 eeprom_write_byte((uint8_t*)(0x1),45);                                  //Waveform period
 eeprom_write_byte((uint8_t*)(0x2),0); 
 eeprom_write_byte((uint8_t*)(0x3),30);                                  //Number of harmonics
 eeprom_write_byte((uint8_t*)(0x4),0);}

enable_pci_on_sw1;
 
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

for (int n = 0; n < num_time_slots*2 ; n++)                               //Calculate amplitude for each time slot
{Time = (float)n/(float)num_time_slots;                                 //0.0 allowed because cos 0 equals unity
amplitude = 0.0;                                                        //Zero allowed for addition (but not multiplication)

for (int p = 1; p <= num_harmonics; p += 1){                            //Sum amplitude of each harmonic
wdr();
if (p==1)Harmon_1 = print_offset + (int)(sin(pi * Time) * 40.0); 
if (p==2)Harmon_2 = print_offset - (int)(sin(pi * Time * 2.0)/2.0 * 40.0); 
if (p==3)Harmon_3 = print_offset + (int)(sin(pi * Time * 3.0) / 3.0 * 40);
if (p==4)Harmon_4 = print_offset - (int)(sin(pi * Time * 4.0)/4.0 * 40.0);

if(p%2)amplitude += sin(pi * Time * (float) p) / (float) p;
if(!(p%2))amplitude -= sin(pi * Time * (float) p) / (float) p; }
amplitude = (int)(amplitude * 40.0) + print_offset;

for(int m = 0; m < 150; m++){
if (m==amplitude)  Serial.write('|');
else if (m==Harmon_1)Serial.write('*');
else if (m==Harmon_2)Serial.write('!');
else if (m==Harmon_3)Serial.write('^');
else if (m==Harmon_4)Serial.write('#');
else if (m == print_offset) {Serial.write('|');}

else  Serial.write(' ');}

_delay_ms(20);

newline_A; wdr();

wdr();}
counter += 1;}

setup_watchdog; SW_reset;}


/*******************************************************************************************************************/
ISR(PCINT2_vect){
  int data;
  if (switch_1_up)return;
  sei();
  disable_pci_on_sw1;

data = PCI_triggers_data_from_PC(digits);
if(!(data)){Timer_T1_sub_with_interrupt(T1_delay_250ms);return;} 

if((switch_2_up) && (switch_3_up)){                                     //Set duty cycle
if(data > 9)data = data%10 + 1;
eeprom_write_byte((uint8_t*)(0x0),data);}
    
 if((switch_2_down) && (switch_3_up))                                   //Set period
 {eeprom_write_byte((uint8_t*)(0x1),data);
 eeprom_write_byte((uint8_t*)(0x2), (data >> 8));}

if((switch_3_down) && (switch_2_up))                                    //Set number of harmonics
{ eeprom_write_byte((uint8_t*)(0x3),data); 
 eeprom_write_byte((uint8_t*)(0x4), (data >> 8));}

SW_reset;}
  


/*******************************************************************************************************************/
int PCI_triggers_data_from_PC(char * num_as_string)  
{int m= 0;
for(int m = 0; m <= 7; m++)num_as_string[m] = 0;
while(1){if (Serial.available()) num_as_string[m++] = Serial.read(); else break;}
if (!(m))return 0;
return atoi(num_as_string);}




/*******************************************************************************************************************/
