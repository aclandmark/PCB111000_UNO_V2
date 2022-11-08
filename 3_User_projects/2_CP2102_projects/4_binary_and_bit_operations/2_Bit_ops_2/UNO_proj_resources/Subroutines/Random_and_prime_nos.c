

unsigned char PRN_8bit_GEN(unsigned char start, char * counter){
    unsigned char bit, lfsr;
    char EEP_offset;

     EEP_offset = eeprom_read_byte((uint8_t*)(0x3EF));
    if(EEP_offset > 4)EEP_offset = 0;

    if((!(*counter)) && (!(start)))   
    {lfsr = eeprom_read_byte((uint8_t*)(0x3F4 - EEP_offset));}
        
    else lfsr = start;
    
    bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1;
    lfsr = (lfsr >> 1) | (bit << 7);
        
    if(!(*counter)){Char_to_PC_Basic(EEP_offset + '0');Toggle_LED_1;
    eeprom_write_byte((uint8_t*)(0x3F4 - EEP_offset),lfsr);}

  return lfsr;}




/*****************************************************************/

unsigned int PRN_16bit_GEN(unsigned int start, unsigned char *PRN_counter){				//Pseudo random number generation 
unsigned int bit, lfsr, eep_address;													//The calling routine provides memory space for PRN_counter
unsigned char eep_offset;																//The subroutine provides it for every thing else

eep_offset = eeprom_read_byte((uint8_t*)(0x3EF));										//Three pairs of eeprom registers are reserved to backup PRN numbers
if(eep_offset > 4) eep_offset = 0;

if((!(*PRN_counter)) && (!(start)))														//Only read the EEPROM the first time a program calls this subroutine
{lfsr = (eeprom_read_byte((uint8_t*)(0x3F5 - eep_offset)) << 8) + 
eeprom_read_byte((uint8_t*)(0x3F4 - eep_offset));}										//Data saved to EEPROM survives resets including POR (power on reset)

else lfsr = start;																		//Use previous PRN value to generate the next one

bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
lfsr = (lfsr >> 1) | (bit << 15);

*PRN_counter += 1;	 
if(*PRN_counter == 255)

{eeprom_write_byte((uint8_t*)(0x3F5 - eep_offset),(lfsr>>8));							//Save every 16th PRN number to EEPROM
eeprom_write_byte((uint8_t*)(0x3F4 - eep_offset),lfsr);									//This prevents the display from endlessly repeating
Toggle_LED_1;																			//Note: Saving every one burns out the EEPROM location too quickly
*PRN_counter = 0;
Char_to_PC_Basic(eep_offset + '0');}

return lfsr;}


/************************************************************************/
/*void I2C_Tx_snowstorm_display(void){

int PRN;

while(1){
PRN = PRN_16bit_GEN (0);									//Generate a new PRN (0) tells subroutine to use the EEPROM
I2C_Tx_2_integers(PRN, (PRN<<1));							//Display two "pseudo random numbers"
Timer_T1_sub(T1_delay_100ms);}}

*/


/*****************************************************************/


/************************************************************************/
/*void I2C_Tx_snowstorm_display_UNO(void){

int PRN;

while(1){
PRN = PRN_16bit_GEN_UNO (0);									//Generate a new PRN (0) tells subroutine to use the EEPROM
I2C_Tx_2_integers(PRN, (PRN<<1));							//Display two "pseudo random numbers"
Timer_T1_sub(T1_delay_100ms);}}*/






/*****************************************************************/
void prime_no_generator(int l,int n,  int reg[]){
int i, m;
i=2; 
while (i*i < n*(l+1)){
m=0;
while(i*(i+m) <= n*l)m++; 
while(i*(i+m) <= n*(1+l)){reg[i*(i+m) -1 - n*l] = 0; m++; }	
i++;}}


/*****************************************************************/
void prime_no_generator_plus( int l,int n,  int reg[]){
int i, m;
i=2; 
while (i*i < n*(l+1)){
m=0;
while(i*(i+m) <= n*l)m++; 
if(reg[i*(i+m) -1 - n*l]){
while(i*(i+m) <= n*(1+l)){reg[i*(i+m) -1 - n*l] = 0; m++; }	}
i++;}}


/*****************************************************************/
int Product_search	(int number)	{
int search_array[10];	
int j=0;
int n=10;
int l=0;
while(1){
for(int k =0; k < n; k++) {search_array[k] = k+1+n*l;}
prime_no_generator(l,n,search_array);
j=0; while(search_array[j] == 0)j++;
while (j<=10){if ((search_array[j] >= 2) &&  (number%search_array[j] == 0)){return search_array[j];}j++;}
if(l*10 > number)return 0;
l++;}}



/*****************************************************************/
int Prime_search ( int number, int array_size)	{
int search_array[array_size];	
int j=0;
int l=0;
while (1){
for(int k =0; k < array_size; k++) {search_array[k] = k+1+array_size*l;}
prime_no_generator(l,array_size,search_array);
j=0;
while (j<array_size){if ((search_array[j] >= 2) &&  (number%search_array[j] == 0)){
if (search_array[j]== number) {return 1;} else return 0;}j++;}
if(l*array_size > number)return 0;
l++;}	}
