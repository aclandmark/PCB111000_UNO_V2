

int main (void)           	//Example 4
  { setup_HW_basic;				//Prints text file
  while (!(isCharavailable_Local(65)))  
    Char_to_PC_Local('?');        
  newline_Basic();
  Char_to_PC_Local(Char_from_PC_Local());
  while (1)
  { if (isCharavailable_Local(10))
      Char_to_PC_Local(Char_from_PC_Local());
    else break;
  }
  String_to_PC_Local("Done\r\n");
  SW_reset;
  return 1;}
  
  
  
  
  int main (void)        //Example 5
  { setup_HW_basic;			//Prints text file
  char keypress;
 
  String_to_PC_Basic("abcdefghijklmnopqrstuvwxyz");
 String_to_PC_Basic("\r\nSend_the_hex_file\r\n");

 while (!(isCharavailable_Local(65))) 
    Char_to_PC_Local('?');       
  newline_Basic();
  Char_to_PC_Local(Char_from_PC_Local());
  while (1)
  { if (isCharavailable_Local(10))
      Char_to_PC_Local(Char_from_PC_Local());
    else break;
  }
 String_to_PC_Local("Done\r\n");
  
 while(1); }