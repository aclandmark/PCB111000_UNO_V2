
float FPN_div (float, float);



/********************************************************************************************************************************/
float FPN_add (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_digits_1, FPN_digits_2, FPN_digits_3;
char sign_1, sign_2, sign_3 = '+';
float Result_as_long;

sign_1 = '+';
sign_2 = '+';

FPN_digits_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_digits_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

if (sign_1 == '-')FPN_digits_1 = -FPN_digits_1;
if (sign_2 == '-')FPN_digits_2 = -FPN_digits_2;

twos_expnt_3 = twos_expnt_1;

if (twos_expnt_1 > twos_expnt_2){
  while(twos_expnt_1 > twos_expnt_2){twos_expnt_2 += 1; FPN_digits_2 = FPN_digits_2 >> 1;}twos_expnt_3 = twos_expnt_2;}

if (twos_expnt_1 < twos_expnt_2){
  while(twos_expnt_1 < twos_expnt_2){twos_expnt_1 += 1; FPN_digits_1 = FPN_digits_1 >> 1;}twos_expnt_3 = twos_expnt_1;}

FPN_digits_3 = FPN_digits_1 + FPN_digits_2;

if (sign_1 != sign_2)
{
    if (!(FPN_digits_3))return 0;
  
 if (FPN_digits_3 & (unsigned long) 0x80000000){FPN_digits_3 *= -1; sign_3 = '-';}
while (!(FPN_digits_3 & (0x40000000))){FPN_digits_3 = FPN_digits_3 << 1; twos_expnt_3 -= 1;}}

if(sign_1 == sign_2){

if(twos_expnt_3 == 128){Serial.write("Infinity");SW_reset;}

switch(sign_1){
case '+': break;

case '-': 
FPN_digits_3 =    FPN_digits_1 * -1 + FPN_digits_2 * -1; ; 
sign_3 = '-';break;}

if (FPN_digits_3 & (unsigned long) 0x80000000)
{FPN_digits_3 = (unsigned long)FPN_digits_3  >> 1; twos_expnt_3 += 1;}}

Result_as_long = Assemble_FPN((unsigned long) FPN_digits_3, twos_expnt_3, sign_3);
if (sign_3 == '-')
*(long*)&Result_as_long = *(long*)&Result_as_long | (unsigned long)0x80000000; 
return Result_as_long;}



/********************************************************************************************************************************/
float FPN_sub (float FPN_1, float FPN_2){
*(long*)&FPN_2 = *(long*)&FPN_2 ^ (unsigned long)0x80000000;
return FPN_add (FPN_1, FPN_2);}



/********************************************************************************************************************************/
char FPN_GT (float FPN_1, float FPN_2){
float difference;
difference = FPN_sub(FPN_2, FPN_1);
if(*(long*)&difference & (unsigned long)0x80000000)return 1;
else return 0;}


/********************************************************************************************************************************/
char FPN_LT (float FPN_1, float FPN_2){
float difference;
difference = FPN_sub(FPN_1, FPN_2);
if(*(long*)&difference & (unsigned long)0x80000000)return 1;
else return 0;}



/********************************************************************************************************************************/
char FPN_GT_or_EQ (float FPN_1, float FPN_2){
float difference;
if (*(long*)&FPN_1 == *(long*)&FPN_2)return 1;
difference = FPN_sub(FPN_2, FPN_1);
if(*(long*)&difference & (unsigned long)0x80000000)return 1;
else return 0;}




/********************************************************************************************************************************/
float FPN_div (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_digits_1, FPN_digits_2, FPN_digits_3;
char sign_1, sign_2, sign_3 = '+';
float Result_as_long;
int FPN_shift;

sign_1 = '+';
sign_2 = '+';

FPN_digits_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_digits_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

while (FPN_digits_1 >= FPN_digits_2){FPN_digits_1 >>= 1; twos_expnt_1 += 1;}
FPN_digits_3 = Fraction_to_Binary_Signed(FPN_digits_1, FPN_digits_2);


/*************/
if (sign_1 == sign_2)sign_3 = '+'; else sign_3 = '-';
twos_expnt_3 = twos_expnt_1 - twos_expnt_2;
Serial.write("A");Serial.print(twos_expnt_3);Serial.write("\t");

if(twos_expnt_3 >= 127){Serial.write("Infinity");SW_reset;}

/*if(twos_expnt_3 <= -127){ //Serial.print(twos_expnt_3);	
FPN_shift = twos_expnt_3 * (-1) - 126;	
FPN_digits_3 = (unsigned long)FPN_digits_3 >> FPN_shift;
twos_expnt_3 = -127;}*/
/***********/

Result_as_long = Assemble_FPN((unsigned long) FPN_digits_3, twos_expnt_3,  sign_3);

if (sign_3 == '+');
else
*(long*)&Result_as_long = *(long*)&Result_as_long | (unsigned long)0x80000000; 
return Result_as_long;}



/********************************************************************************************************************************/
float FPN_mult(float FPN_1, float FPN_2){
float Reciprocal;
float Result;
  Reciprocal = FPN_div(1.0, FPN_2);
 Result = FPN_div(FPN_1, Reciprocal);
return Result;}


/***********************************************************************************************************************************/
