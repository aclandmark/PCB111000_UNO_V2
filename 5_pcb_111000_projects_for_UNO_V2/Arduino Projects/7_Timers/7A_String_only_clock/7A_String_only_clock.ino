
//Proj_7A_Demo_Clock_A
//****************************************************************************************************************************


//IT INTRODUCES
//
//Simple clock in which the display is kept up to date by manipulating the string sent to the display every 100 mS.
//
//A watch crystal is not used to ensure accuracy which is therefore poor.
//Pencil and paper recommended to understand string manipulation.


//USER INSTRUCTIONS
//the clock start at time zero.

//Respond to the user prompt by pressing "R" and entering the time when requested or by pressing "r" and letting 

//Unfortunately the I2C bus appears to crash after random intervals of anything between say 10 minutes and an hour
//Proj 7A_1 has therefore been developed which uses the watchdog timer with interrupt to reset both mim-OS and the user project
//when a crash occurs






#include "Proj_7A_header_file_1.h"

char digits[8];


//Enter main routine here




/***********************************************************************************************************************/
void Inc_display(void){
 if (deci_SecsH < '9') deci_SecsH++; 
  else {deci_SecsH = '0'; if ((SecsL) < '9') SecsL++;   
              else {SecsL = '0'; if (SecsH < '5') SecsH++; 
                          else {SecsH = '0';  if ((MinsL) < '9') MinsL++; 
                                      else {MinsL = '0'; if (MinsH < '5') MinsH++; else {MinsH = '0';

 switch (HoursH){ case '0': case '1': if ((HoursL & 0x7F) < '9')HoursL++; else {HoursL='0'; HoursH++;} break;
 case '2': if((HoursL) < '3')HoursL++; else {HoursL='0'; HoursH='0';} break;}

 }  //  update deci_Secs, seconds units and tens, minutes units and tens and hours
 }  //  update deci_Secs, seconds units and tens and minutes units and tens
 }  //  update deci_Secs, seconds units and tens and minutes units
 }  //  update deci_Secs and seconds units and tens
 }  //  update deci_Secs and seconds units
I2C_Tx_8_byte_array(digits);}



/***********************************************************************************************************************/
void set_time(void){

for(int m = 0; m <= 7; m++)digits[m] = 0;
Serial.write("Enter start time Hours, Minutes and Seconds\
\r\n(24 hour clock with no spaces). Terminate with cr\r\n");

while(isCharavailable_A(50) == 0){Serial.write("T?  ");}

digits[7] = Serial.read();I2C_Tx_8_byte_array(digits);
for (int m = 0; m<=4; m++){while(isCharavailable_A(5) == 0);
if(m == 4){digits[2] = Serial.read();deci_SecsH = '0'; deci_SecsL = '0';}
else digits[6 - m] = Serial.read(); 
I2C_Tx_8_byte_array(digits);}

waitforkeypress_A();
I2C_Tx_any_segment_clear_all();
_delay_ms(50);}




/***********************************************************************************************************************/
