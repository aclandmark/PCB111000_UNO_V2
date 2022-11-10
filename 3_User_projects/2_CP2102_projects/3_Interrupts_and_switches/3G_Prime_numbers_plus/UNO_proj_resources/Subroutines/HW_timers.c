
#define T0_delay_10ms 	7,178

#define T1_delay_10ms	2,0xD8EF
#define T1_delay_50ms 5,0xFE78
#define T1_delay_100ms 5,0xFCF2
#define T1_delay_250ms 5,0xF85F
#define T1_delay_500ms 5,0xF0BE

#define T1_delay_1sec 5,0xE17D
#define T1_delay_2sec 5,0xC2FB
#define T1_delay_4sec 5,0x85F7




/**********************************************************************************************************************************************************************************/
void Timer_T1_sub(char Counter_speed, unsigned int Start_point){ 
TCNT1H = (Start_point >> 8);
TCNT1L = Start_point & 0x00FF;											//TCNT1 counts up from its start_point to 0x10000 (zero)
TIFR1 = 0xFF;															//Clear timer interrupt flags
TCCR1B = Counter_speed;	
while(!(TIFR1 && (1<<TOV1)));											//Wait here for timer to overflow (count from 0xFFFF to zero)
TIFR1 |= (1<<TOV1); 													//Clear overflow flag
TCCR1B = 0;}															//Halt counter



/**********************************************************************************************************************************************************************************/
void Timer_T0_10mS_delay_x_m(int m)
{for (int n = 0; n < m; n++)
{Timer_T0_sub(T0_delay_10ms);wdr();}}									//Reset WDTimer every 10 mS



/**********************************************************************************************************************************************************************************/
void Timer_T0_sub(char Counter_speed, unsigned char Start_point){ 		//Operation is same as that of Timer 1
TCNT2 = Start_point;
TCCR2B = Counter_speed;
while(!(TIFR2 & (1<<TOV2)));
TIFR2 |= (1<<TOV2); TCCR2B = 0;}



