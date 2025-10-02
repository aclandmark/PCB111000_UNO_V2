
int main (void)                     //Example 9
  { char char_counter = 0;			//.hex file printout with text in clear
    char keypress;
    char line_len;
    char High_byte=0;
    char askii_char;
   
    setup_HW_basic;
    String_to_PC_Basic("ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n\
Send hex file\r\n");
          
  while (!(isCharavailable_Local(65)))  
    Char_to_PC_Local('?');    
  newline_Basic();
  Char_from_PC_Local();

  while (1)
  { if (isCharavailable_Local(10))
        {keypress =  Char_from_PC_Local();
      if(keypress ==':')char_counter = 0;
      if (char_counter == 1)line_len = (keypress - '0')*0x10;
      if (char_counter == 2)line_len += keypress - '0';
      char_counter += 1;
     if((char_counter >=10) && (char_counter < ((line_len*2) + 10)))
            
      {if (!(char_counter%2))
      {if ((keypress >= '2') &&  (keypress <= '7'))High_byte = keypress; 
            else {Char_to_PC_Local(keypress);}}
      if (char_counter%2)
      {if (!(High_byte))Char_to_PC_Local(keypress);  
            else{
              
              if (keypress >='A')keypress -= 7;
              askii_char = ((High_byte-'0') * 0x10)+ keypress - '0';
            Char_to_PC_Basic(askii_char);High_byte=0;}}
      
        }else {Char_to_PC_Local(keypress);}}
       else break;
       }
  String_to_PC_Local("Done\r\n");
  SW_reset;
  return 1;}
