


/**************Proj_2C1_random_LED kaleidoscope***************************/




#include "Proj_2C1_header_file.h"

char display_bkp[7];                                                            //One element to backup each segment letter 

int main (void){
char segment=0, digit_num=0, seg_counter = 0,direction = 0; 
unsigned char PRN_counter = 0;
unsigned int PRN;

setup_HW_basic;

wdt_enable(WDTO_2S);                                                           //WDT prevents display from being completed in either direction

I2C_Tx_any_segment_clear_all();                                                //Initialise display
PRN = PRN_16bit_GEN (0,  &PRN_counter);
UCSR0B |= (1 << RXCIE0);                                                      //Set Interrupt on key press (for test purposes only)
sei();                                                                        //Global enable interrupt

while(1){                                                                     //Generate pattern
while(seg_counter < 56){                                                      //There are 56 segments in total  
PRN = PRN_16bit_GEN (PRN,  &PRN_counter); 
segment = (PRN%7) + 'a';
PRN = PRN_16bit_GEN (PRN,  &PRN_counter);
digit_num = PRN%8;
                                                                              //Continue statements skip back to the top of the while-loop
                                                                              //This is to ensure segments are not turned-off before 
                                                                              //all have been turned on.
                            
if ((!(direction)) && (display_bkp[segment - 'a'] & (1 << digit_num))) {PRN_counter -= 1; continue;}
if ((direction) && (!(display_bkp[segment - 'a'] & (1 << digit_num)))) {PRN_counter -= 1; continue;}

I2C_Tx_any_segment(segment, digit_num);                                      //Update display
backup_the_display(segment, digit_num);                                     //keep backup up to date
Timer_T0_10mS_delay_x_m(5);  wdr();                                         //delay and reset watch dog
seg_counter += 1;}
direction ^= 1;                                                             //Toggle the direction_counter value
seg_counter = 0;
Timer_T0_10mS_delay_x_m(100);}}                                             //Just pause before toggling leds off one at a time


/*****************************************************************/
void backup_the_display(char segment, char digit_num){
display_bkp[segment - 'a'] = display_bkp[segment - 'a'] ^ (1 << digit_num);}


/*****************************************************************/
ISR(USART_RX_vect){
  Char_from_PC_Basic();
I2C_Tx_any_segment_clear_all();
sei();while(1);}
