long int_divide_Local(long A, long B, int * binary_pt_counter)    
  {
    long Div, mod;
    long Int_result;

 *binary_pt_counter = 0;
 
real_divide_Local(A, B, &Div, &mod);                      //Perform the first division to get the integer result plus remainder
 Int_result = Div;
while (!(Int_result & 0x1000000))                   //Reserve 25 bits for the result of the division      
{real_divide_Local(mod*2, B, &Div, &mod);                 //Double the remainder and repeat the division.
 Int_result = Int_result <<1;                       //For each division shift result one place left
    if (Div) Int_result = Int_result | 1;           //If the result was one place a one in the bit just vacated
    *binary_pt_counter -= 1;}                            //The number of bits to the right of the binary point
   Int_result += 1;                                 //Add 1 to round up the result
   Int_result = Int_result>> 1;                     //Reduce number to 24 bits
  *binary_pt_counter += 1;
   
  return Int_result; } 



/*****************************************************************************************************************************/
void real_divide_Local(long A, long B, long *Div, long *mod)      
  {*Div = A/B;
  *mod = A%B;}