#include "UNO_proj_resources/Header_files/Basic_HW_setup.h"
#include "UNO_proj_resources/Header_files/I_O_from_switches_UNO.h"

#include "UNO_proj_resources/I2C_Subroutines/I2C_subroutines_1.c"
#include "UNO_proj_resources/I2C_Subroutines/I2C_subroutines_2.c"
#include "UNO_proj_resources/I2C_Subroutines/I2C_slave_Rx_Tx.c"

#include "UNO_proj_resources/Subroutines/Basic_HW_plus_Timer.c"
#include "UNO_proj_resources/Subroutines/Basic_PC_comms.c"
#include "UNO_proj_resources/Subroutines/Random_and_prime_nos.c"


/************************************************************************************************************************************/
#define shift_PORT_1 \
Random_num = PRN_16bit_GEN(Random_num, &PRN_counter);\
shift_display = (Random_num%3) + 1;\
for (int m = 0; m < shift_display; m++)\
{if (PORT_1 == 0x8000)\
PORT_1 = 1;\
else PORT_1 = PORT_1 << 1;}


/************************************************************************************************************************************/
#define shift_PORT_2 \
if (PORT_2 == 0x8000)\
PORT_2 = 1;\
else PORT_2 = PORT_2 << 1;


/************************************************************************************************************************************/
#define flash_display \
I2C_Tx_2_integers(PORT_1, 0);\
Timer_T0_10mS_delay_x_m(16);\
I2C_Tx_2_integers(0, PORT_2);\
Timer_T0_10mS_delay_x_m(16);




/*************************************************************************************************************************************/
