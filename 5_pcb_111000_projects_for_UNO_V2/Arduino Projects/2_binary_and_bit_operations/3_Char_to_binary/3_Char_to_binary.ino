

/*
Uses the display to illustrate conversion between 8 bit binary and decimal numbers
*/



#include "Char_to_binary_header.h"


char digit=1, op, mode;

//Type main routine here


/**************************************************************************************************/
ISR(PCINT0_vect){                                          //sw3 (PB2) interrupt service routine
if(switch_3_up)return;                                     //Ignore sw3 key release
if((switch_1_down) || (switch_2_down))return;              //Ignore if sw1 or 2 are still down
_delay_ms(100);
if (mode == 's')  
{mode = 'u'; I2C_Tx_display_char(digit,mode);return;}      //toggle display from signed to unsigned
if (mode == 'u')
{mode = 's'; I2C_Tx_display_char(digit,mode);return;}}     //and visa-versa



ISR(PCINT2_vect) {                                        //sw1 (PD2) and sw2 (PD7) interrupt
if(switch_3_down)return;                                  //Ignore if sw3 is still down
if((switch_1_up) && (switch_2_up)){op = 0; return;}       //Both switches up

if(switch_1_down) {op = 1; return;}                       //Digits increment
if(switch_2_down) {op = 2; return;}}                      //Digits decrement






/*****************************************************************************************************/
