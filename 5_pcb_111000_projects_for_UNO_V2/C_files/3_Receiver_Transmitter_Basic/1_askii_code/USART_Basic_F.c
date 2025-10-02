#define Array_size 300  

int main (void)					//Example 10
  { char keypress;
  int line_counter;
   char char_store [Array_size];
  int char_store_pointer;
   char print_cntrl;
   
    setup_HW_basic;

  line_counter = 0;
  char_store_pointer = 0;
  print_cntrl = 0;

    if(watch_dog_reset != 1)
    {String_to_PC_Basic("Send hex file");
    while (!(isCharavailable_Local(65)));}
  else
    {while (!(isCharavailable_Local(65))) 
    Char_to_PC_Local('?');}  
  
  newline_Basic();
  Char_to_PC_Local(Char_from_PC_Local());
  while (1)
  {if (!(isCharavailable_Local(10)))break;
        
      {keypress = Char_from_PC_Local();
      Char_to_PC_Local(keypress);
      if (keypress == '\n') line_counter += 1;
      if (line_counter ==10)UCSR0B &= (~(1<< TXEN0));
     char_store[char_store_pointer++] = keypress; 
      char_store_pointer %= Array_size;}}
      
  UCSR0B |= (1<< TXEN0);
  newline_Basic(); 

  for(int m = 0; m <Array_size; m++){
  if ((char_store[m] == '\r')||(char_store[m] == '\n'))
      print_cntrl = 1; 
  if ((m < char_store_pointer) && (print_cntrl == 1))
      Char_to_PC_Basic(char_store[m]);}

  newline_Basic();
  String_to_PC_Basic("File size: ");
  Int_to_PC_Basic (line_counter);
  String_to_PC_Basic("  lines\r\n ");
  
  SW_reset;
  return 1;}