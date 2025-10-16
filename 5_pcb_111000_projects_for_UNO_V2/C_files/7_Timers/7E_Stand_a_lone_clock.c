int main (void){
char display_mode;

setup_HW_A;
if (switch_1_down)
{User_prompt_A; 
User_instructions;
while(switch_1_down);}
time_from_IO();
I2C_Tx_OS_timer(AT_clock_mode, start_time);                                 //Send Start clock command (AT clock mode is 7)

//Copy this bit from "7D_mini_OS_clock_with_adjustment.ino"
/******************************************************************************************************************************/
display_mode = 0;

while(1){                                                                 //Operation continuously cycles around this loop 
Timer_T0_10mS_delay_x_m(10);
switch (display_mode){                                                                

case 0: if(switch_3_down){Timer_T0_10mS_delay_x_m(50); display_mode = 'A';}
    if(switch_1_down){display_mode = 'B';I2C_Tx_Clock_command(hide_clock);while(switch_1_down);}
    if(switch_2_down){display_mode = 'C';I2C_Tx_Clock_command(pause_clock);while(switch_2_down);}break;

case 'A': if(switch_3_up)display_mode = 1; else {while(switch_3_down); display_mode = 2;} break;
case 'B':if(switch_1_down){display_mode = 0;I2C_Tx_Clock_command(display_clock);while(switch_1_down);}break;
case 'C':if(switch_2_down){display_mode = 0; I2C_Tx_Clock_command(display_clock);while(switch_2_down);}break;


case 1: if (switch_2_down){I2C_Tx_Clock_command(increment_seconds);Timer_T0_10mS_delay_x_m(20);}
    if (switch_1_down){I2C_Tx_Clock_command(increment_minutes);Timer_T0_10mS_delay_x_m(20);}
    if(switch_3_down){ while (switch_3_down);display_mode = 0; }break;
    
case 2: if (switch_2_down){I2C_Tx_Clock_command(decrement_seconds);Timer_T0_10mS_delay_x_m(20);}
    if (switch_1_down){I2C_Tx_Clock_command(decrement_minutes);Timer_T0_10mS_delay_x_m(20);}
    if(switch_3_down){while (switch_3_down);display_mode = 0; }break;}}}  
