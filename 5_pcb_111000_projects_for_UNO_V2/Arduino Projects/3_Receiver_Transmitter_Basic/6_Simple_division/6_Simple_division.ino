


//Introducing the divide and modulo operations 


#include "Simple_division_header.h"


//Type main routine here




/***********************************************************************************************/
  long divide(long A, long B, long *Div, long *mod, int no_decimal_places)
  {*Div = A/B;
  *mod = A%B;
  no_decimal_places -= 1;
  return no_decimal_places; } 



  /****************************************************************************************************/
