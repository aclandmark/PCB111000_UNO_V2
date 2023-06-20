
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

int print_offset = 100;                                                 //Used to fill screen
char digits[12];                                                       //Array used to collect keyboard string

int main (void){

float pi = 3.14159;
float Time;                                                            //< 0 Time >= 1 (Assumes a waveformperion T of unit
int amplitude;                                                       //Synthesized amplitude at and any time
int num_time_slots;                                                    //45
int num_harmonics;                                                     //45
int Harmon_2, Harmon_4, Harmon_6, Harmon_8, Harmon_10;
int Harmon_2_offset, Harmon_4_offset, Harmon_6_offset, Harmon_8_offset, Harmon_10_offset;

setup_HW_with_reset_analysis;

set_up_PCI;
setup_watchdog;

if(reset_status != 2)eeprom_write_byte((uint8_t*)(0x0),0); 

enable_pci_on_sw1;
 
num_time_slots = 45;
num_harmonics = 45;


newline_A;
for(int n = 0; n <= 100; n++){if (n<40)Serial.write (' ');              //Print plot axis
else Serial.write('_');}
newline_A;

while(1){

for (int n = 0; n < num_time_slots ; n++)                               //Calculate amplitude for each time slot
{Time = (float)n/(float)num_time_slots;                                 //0.0 allowed because cos 0 equals unity
amplitude = 0.0;                                                        //Zero allowed for addition (but not multiplication)

for (int p = 1; p <= num_harmonics; p += 1){                            //Sum amplitude of each harmonic
wdr();
if (p==2){Harmon_2 = - (int)(sin(2.0 * pi * Time * 2.0)/2.0 * 80.0); 
          Harmon_2_offset =  - (int)(sin((0.5 + 2.0 * pi * Time) * 2.0)/2.0 * 80.0);}
if (p==4){Harmon_4 =  - (int)(sin(2.0 * pi * Time * 4.0)/4.0 * 80.0);
          Harmon_4_offset =  - (int)(sin((0.5 + 2.0 * pi * Time) * 4.0)/4.0 * 80.0);}
if (p==6){Harmon_6 =  - (int)(sin(2.0 * pi * Time * 6.0)/6.0 * 80.0);
          Harmon_6_offset =  - (int)(sin((0.5 + 2.0 * pi * Time) * 6.0)/6.0 * 80.0);}
if (p==8){Harmon_8 =  - (int)(sin(2.0 * pi * Time * 8.0)/8.0 * 80.0);
         Harmon_8_offset =  - (int)(sin((0.5 + 2.0 * pi * Time) * 8.0)/8.0 * 80.0);}
if (p==10){Harmon_10 =  - (int)(sin(2.0 * pi * Time * 10.0)/10.0 * 80.0);
          Harmon_10_offset =  - (int)(sin((0.5 + 2.0 * pi * Time) * 10.0)/10.0 * 80.0);}
         
if(!(p%2)){amplitude -= int(sin(2.0 * pi * Time * (float) p) / (float) p * 80.0);}}

switch (eeprom_read_byte((uint8_t*)(0x0))){
  case 0: Display_1(amplitude );break;
  case 1: Display_2(amplitude, Harmon_2 );break;
  case 2: Display_2(amplitude, Harmon_4 );break;
  case 3: Display_2(amplitude, Harmon_6 );break;
  case 4: Display_2(amplitude, Harmon_8 );break;
  case 5: Display_1(Harmon_2 + Harmon_4 );break;
  case 6: Display_1(Harmon_2 + Harmon_4 + Harmon_6 );break;
  case 7: Display_1(Harmon_2 + Harmon_4 + Harmon_6 + Harmon_8 );break;
  case 8: Display_1(Harmon_2 + Harmon_4 + Harmon_6 + Harmon_8 + Harmon_10 );break;
  case 9: Display_2(amplitude, Harmon_2_offset + Harmon_4_offset + Harmon_6_offset + Harmon_8_offset + Harmon_10_offset );break;}

_delay_ms(20);
newline_A; wdr();}}

setup_watchdog; SW_reset;}


/*******************************************************************************************************************/
ISR(PCINT2_vect){
  int data;
  if (switch_1_up)return;
  sei();
  disable_pci_on_sw1;

data = PCI_triggers_data_from_PC(digits);
if(!(data - '0')){Timer_T1_sub_with_interrupt(T1_delay_250ms);return;} 
if(data < 10)eeprom_write_byte((uint8_t*)(0x0),data);
else Serial.print(data);

SW_reset;}
  

ISR(TIMER1_OVF_vect) {TIMSK1 &= (~(1 << TOIE1)); enable_pci_on_sw1;Serial.write("!?");}


/*******************************************************************************************************************/
int PCI_triggers_data_from_PC(char * num_as_string)  
{int m= 0;
while(1){if (Serial.available()) {num_as_string[m] = Serial.read(); 
if(decimal_digit_A(num_as_string[m]))m += 1;} else break;}
num_as_string[m] = '\0';
if (!(m))return '0';
return atoi(num_as_string);}



/***********************************************************************************************************************/
void Display_1(int amplitude){
char print_control= 0;
amplitude += print_offset;
for(int m = 0; m < 175; m++){
if (m==amplitude){Serial.write('|');print_control += 1;}
if ((m == print_offset) && (!(print_control))){Serial.write(':');print_control += 1;}
if(print_control)print_control= 0; else  Serial.write(' '); }}



/*******************************************************************************************************************/
void Display_2(int amplitude,int Harmon_2 ){
char print_control= 0;

amplitude += print_offset;
Harmon_2 += print_offset;
print_control= 0;
for(int m = 0; m < 175; m++){
if (m==Harmon_2){Serial.write('|');print_control += 1;}
if (m==amplitude){Serial.write('!');print_control += 1;}
if ((m == print_offset) && (!(print_control))){Serial.write(':');print_control += 1;}
if(print_control){print_control= 0;} else  Serial.write(' '); }}




/**********************************************************************************************************************/
