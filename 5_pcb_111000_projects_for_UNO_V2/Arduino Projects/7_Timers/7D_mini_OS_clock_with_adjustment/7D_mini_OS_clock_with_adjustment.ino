
//Proj_7D_Crystal_clock_with adjustment


//As for Proj_7C the mini-OS provides a crystal controlled clock with moderately good accuracy
//which can be operated independently of a PC.

//IT INTRODUCES

//The following mini-OS clock commands

  //I2C_Tx_Clock_command(hide_clock);
  //I2C_Tx_Clock_command(pause_clock);
  //I2C_Tx_Clock_command(display_clock);
  //I2C_Tx_Clock_command(increment_seconds);
  //I2C_Tx_Clock_command(increment_minutes);
  //I2C_Tx_Clock_command(decrement_seconds);
  //I2C_Tx_Clock_command(decrement_minutes);


  //USER INSTRUCTIONS

//During operation the three switches are continuously polled.

//To control the display:
  //Press sw1 to toggle the display ON and OFF
 //Press sw2 to pause or resume the clock

//To adjust the clock:
  //Pulse sw2 then press sw1 and sw3 to advance the time
  //or press and hold sw2 (for 500ms)
  //then press sw1 and sw3 to retard the time
  //Always pulse sw2 when the time is correct

 //Switch location SW1(PD2) - SW2(PD7) – SW3(PB2)
#define SB_delay Timer_T0_10mS_delay_x_m(10);


#define message_1 \
"\r\nCRYSTAL CLOCK with adjustment\r\n\
Press sw1 to toggle the display on and off\r\n\
Press sw2 to pause or resume the clock\r\n\
To adjust the clock:\r\n"

#define message_2 \
"pulse sw3 then press sw1 and sw2 to advance the time\r\n\
or press and hold sw3 (for 500ms)\r\n\
then press sw1 and sw2 to retard the time\r\n\
Always pulse sw3 when the time is correct\r\n"

#define User_instructions \
Serial.write(message_1);\
Serial.write(message_2);

#include "Proj_7D_header_file_1.h"

char start_time[8],  display_mode;
unsigned char input_mode;
char keypress;


//Enter main routine here



while(1){

switch (display_mode){                                    //Beware of switch bounce

case 0: if(switch_3_down){Timer_T0_10mS_delay_x_m(50); display_mode = 'A';}
    if(switch_1_down){display_mode = 'B';I2C_Tx_Clock_command(hide_clock);while(switch_1_down);}
    if(switch_2_down){display_mode = 'C';I2C_Tx_Clock_command(pause_clock);while(switch_2_down);}SB_delay;break;

case 'A': if(switch_3_up)display_mode = 1; else {while(switch_3_down); display_mode = 2;} SB_delay;break;
case 'B':if(switch_1_down){display_mode = 0;I2C_Tx_Clock_command(display_clock);while(switch_1_down);}SB_delay;break;
case 'C':if(switch_2_down){display_mode = 0; I2C_Tx_Clock_command(display_clock);while(switch_2_down);}SB_delay;break;


case 1: if (switch_2_down){I2C_Tx_Clock_command(increment_seconds);Timer_T0_10mS_delay_x_m(20);}
    if (switch_1_down){I2C_Tx_Clock_command(increment_minutes);Timer_T0_10mS_delay_x_m(20);}
    if(switch_3_down){ while (switch_3_down);display_mode = 0; }SB_delay;break;
    
case 2: if (switch_2_down){I2C_Tx_Clock_command(decrement_seconds);Timer_T0_10mS_delay_x_m(20);}
    if (switch_1_down){I2C_Tx_Clock_command(decrement_minutes);Timer_T0_10mS_delay_x_m(20);}
    if(switch_3_down){while (switch_3_down);display_mode = 0; }SB_delay;break;}}}  
 


/**********************************************************************************************************************/
//Type in subroutine void timer_utoa(char n)


void timer_utoa(char n){                //converts an unsigned number to a string of digits
if (n>=10)
{charL = n%10;
charH = (n-charL)/10;
charL = charL + '0';
charH = charH + '0';}
else
{charH = '0'; charL = n + '0';}}




/************************************************************************************************************************/
