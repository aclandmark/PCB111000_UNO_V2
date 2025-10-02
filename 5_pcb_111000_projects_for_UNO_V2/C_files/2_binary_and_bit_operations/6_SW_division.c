
//Divide routine


while (!(dividend_mem & (mask))){dividend_mem = dividend_mem << 1; num_bits -=1;}
num_bits_mem = num_bits;

while ((dividend >> 1) >= divisor) {dividend =  (dividend >> 1); num_bits -= 1;}
for (int m = 0; m < num_bits; m++) dividend_mem = dividend_mem << 1;

for (int p = 0; p < (num_bits_mem - num_bits ); p++){
q = p;
  
dividend = Logic_subtract_Num1_Num_2(dividend, divisor);

result = (result << 1) | 1;   
dividend = dividend << 1; 
if (dividend_mem & mask)dividend |= 1;  

dividend_mem = dividend_mem << 1;

while (divisor > dividend) {result = (result << 1);  
p += 1; q += 1;
if (p == (num_bits_mem - num_bits))break; 
 dividend = dividend << 1; 
  if (dividend_mem & mask)dividend |= 1;
  dividend_mem = dividend_mem << 1;  }} 

if (q != (num_bits_mem - num_bits)){result = (result << 1) | 1;
modulo = dividend - divisor;}
else modulo = dividend;