#include "UNO_proj_resources/Header_files/Basic_HW_setup.h"
#include "UNO_proj_resources/Header_files/I_O_from_switches_UNO.h"

#include "UNO_proj_resources/I2C_Subroutines/I2C_subroutines_1.c"
#include "UNO_proj_resources/I2C_Subroutines/I2C_subroutines_2.c"
#include "UNO_proj_resources/I2C_Subroutines/I2C_slave_Rx_Tx.c"

#include "UNO_proj_resources/Subroutines/Basic_HW_plus_Timer.c"
#include "UNO_proj_resources/Subroutines/Basic_PC_comms.c"
#include "UNO_proj_resources/Subroutines/Random_and_prime_nos.c"



#define Initialise_display \
PORT_1 = 0b0000000000000001;\
PORT_2 = 0b1000000000000000;\
I2C_Tx_2_integers(PORT_1, PORT_2);



#define address_of_last_string \
((eeprom_read_byte((uint8_t*)(0x3E9))) << 8)\
+ eeprom_read_byte((uint8_t*)(0x3EA))

#define save_string_address \
eeprom_write_byte((uint8_t*)(0x3E9), eep_address >> 8);\
eeprom_write_byte((uint8_t*)(0x3EA), eep_address);


#define inc_display \
*PORT_1 = *PORT_1 << 1;\
*PORT_2 = *PORT_2 >> 1;\
I2C_Tx_2_integers(*PORT_1, *PORT_2);\
if (*PORT_2 == 1)\
{*PORT_1 = 0b0000000000000001;\
*PORT_2 = 0b1000000000000000;}
/************************************************************************************************************************************/
