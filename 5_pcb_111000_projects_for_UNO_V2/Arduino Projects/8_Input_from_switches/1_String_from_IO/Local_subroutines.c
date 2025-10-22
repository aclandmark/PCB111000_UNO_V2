
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define switch_1_down ((PIND & 0x04)^0x04)
#define switch_2_down ((PIND & 0x80)^0x80)

void I2C_Tx_any_segment_clear_all(void);
void I2C_Tx_8_byte_array(char *);

extern char digits[8];

void rotate_display(){   
  char display_string[8];
  char num_length = 0;
  int p;
  
for(int m = 0; m < 8; m++)if (!(digits[m]))break;
else num_length += 1;

I2C_Tx_any_segment_clear_all();
for(int m = 0; m <=7; m++)display_string[m] = 0;
p = 0;
while(1){
display_string[0] = digits[num_length - p%(num_length+1)]; p += 1;
I2C_Tx_8_byte_array(display_string);_delay_ms(100);

for(int m = 0; m < 7; m++){
display_string[7-m] = display_string[7-m-1];}

while(switch_2_down);

if(switch_1_down) break;}
}



/************************************************************************/
