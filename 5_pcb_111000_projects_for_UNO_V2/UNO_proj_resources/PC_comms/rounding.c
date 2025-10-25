

void real_divide(long, long, long *, long *) ;


void Real_num_to_PC_no_rounding(long int_num, int twos_expt, char decimal_places, char * num_string){
long  divisor;
long Div, mod;

Serial.write('\t');
 
 if (twos_expt < 0){
 divisor = 1;
 twos_expt = twos_expt * (-1);

 while (twos_expt >= 31){twos_expt -= 1; int_num = int_num >> 1;}
 for (int m = 0; m < twos_expt; m++) divisor *= 2;

 real_divide(int_num, divisor, &Div, &mod); 
 Int_Num_to_PC_A(Div, num_string, ' ');
 Serial.write('.');
 for(int m = 0; m < decimal_places; m++){

 while (mod >= 0x7FFFFFFF/10){mod = mod/10; divisor /= 10;}
  
 real_divide(mod*10, divisor, &Div, &mod);
 Serial.write(Div + '0'); }}

else {while(twos_expt) {int_num *= 2; twos_expt -= 1;}Int_Num_to_PC_A(int_num, num_string, ' ');}}


/********************************************************************************************************************************************/
void Real_num_to_string_with_rounding(char * num_string, long int_num, int twos_expt, char decimal_places){
long  divisor;
long Div, mod;
char  string_offset;
char string_length;
char leading_zeros = 0;
char digit_counter;


/**************************************************************************************************************************************/
if (twos_expt >= 0) {while(twos_expt) 
{int_num *= 2; twos_expt -= 1;}
Int_Num_to_PC_A(int_num, num_string, ' ');}		//Int_to_PC_Basic(int_num);}                                                       //For very large numbers division is not required.

 /***************************************************************************************************************************************/
 if (twos_expt < 0){
 divisor = 1;
 twos_expt = twos_expt * (-1);                                                  // We divide not multiply by 2^twos_expt

 while (twos_expt >= 31){twos_expt -= 1; int_num = int_num >> 1;}               //Long numbers only have room for 32 bits
 for (int m = 0; m < twos_expt; m++) divisor *= 2;                              //Convert 2^twos_expt to a number (i.e. 2^3 to 8) such as 2, 4, 8, 16....

/*****************************Start as for Prog B_FPN_to PC by obtaining the integer result and remainder*******************************************/
real_divide(int_num, divisor, &Div, &mod);                                   //Divide the integer part by the exponent part to get the integer result

ltoa (Div, num_string, 10);                                                     //Populate string with the integer result (ltoa: "long to askii" is a C library function)
string_offset = 0;                                                              //Calculate the number of digits in the string
while(num_string[string_offset])string_offset += 1;                             //Do not add the decimal point                          

if (Div){
/*********************************************Calculate as many the decimal points as required by the user************************************************/
for(int m = 0; m <= decimal_places; m++){                                       //Obtain one extra decimal place for rounding
if (string_offset == 9){decimal_places = m-1; break;}
real_divide(mod*10, divisor, &Div, &mod);                                        //Place decimal digits in num_string along side the integer result and decimal place
num_string[string_offset++] = Div + '0';   }                                    //String offset points to the next free location in num_string
num_string[string_offset] = 0;                                                  //Terminate the string       
ltoa ((atol(num_string) + 6), num_string, 10);                                    //Use C function Askii to Long Int to convert num_string to an integer, add 5 and convert it back to a string

string_offset = 0; 
while(num_string[string_offset])string_offset += 1;                             //Calculate the number of digits in the string
string_offset -=1;

for(int m = 0; m <(string_offset - decimal_places); m++)                        //Print out the digits to the left of the decimal point
Serial.write (num_string[m]);
Serial.write('.');
for(int m = (string_offset - decimal_places); m < string_offset; m++) 
Serial.write (num_string[m]);  }                                             //Print out the digits to the right of the decimal point


/*************************Rounding for numbers like 0.00567 i.e. positive numbers less than unity****************************************************/
else{                                                                         //We have to compute the number of leading zeros
leading_zeros = 1;
digit_counter = 0;
for(int m = 0; m <= decimal_places; m++){    
while (mod >= 0x7FFFFFFF/10){mod = mod/10; divisor /= 10;}                     //For small numbers the remainder times 10 may may be two large  
     
real_divide(mod*10, divisor, &Div, &mod);       
num_string[string_offset++] = Div + '0';      
if(Div) digit_counter += 1; 
if ((!(Div)) && (leading_zeros) && (!(digit_counter)))
leading_zeros += 1;}                                                            //Number of leading zeros
num_string[string_offset] = 0;                                                  //Terminate the string


/*******************************************************************************************************************************************************/
int num_pre_rounding = atol(num_string);                                        //Rounding can produce an extra digit
int num_post_rounding = num_pre_rounding + 6;                                             //should really be 5
char num_digits_pre_rounding = 0;
char num_digits_post_rounding = 0;

while(num_pre_rounding/10)
{num_digits_pre_rounding += 1;  num_pre_rounding = num_pre_rounding/10;} 
while(num_post_rounding/10)
{num_digits_post_rounding += 1; num_post_rounding = num_post_rounding/10;}

if (num_digits_post_rounding > num_digits_pre_rounding)leading_zeros -=1;       //Because of the extra digit the number of leading zeros msut be reduced by 1

ltoa ((atol(num_string) + 6), num_string, 10);                                  //should really BE 5

string_offset = 0; while (num_string[string_offset])string_offset += 1;         //Remove the final unwanted digit 
string_offset -= 1;
num_string[string_offset] = 0;

if (!(leading_zeros))Serial.write("\t\t1.0");
else {Serial.write("\t\t0.");
for(int m = 0; m < (leading_zeros-1); m++)Serial.write('0');                  //Print the leading zeros and decimal point
Serial.write(num_string);}                                                   //Print the non zero decimal places
}}}


/*****************************************************************************************************************************************************************/