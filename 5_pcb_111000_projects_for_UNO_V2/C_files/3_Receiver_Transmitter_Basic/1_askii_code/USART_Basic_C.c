

int main (void)                //Example 6
  { setup_HW_basic;         //Exploring program memory
  char *mem_add;
 wdt_enable(WDTO_120MS);
 while(switch_2_down)wdr();
  
  
  String_to_PC_Local("\0Null string\r\n");
  String_to_PC_Local("Defining and using text strings\r\n\r\n");
  const char *message_1 = "Hello world\r\n";
  const char *message_2 = "Sending text to a PC\r\n";
  const char message_3[] = "Writing C programs and\r\n";
  const char message_4[] = "Uploading them to a device\r\n\r\n";
  String_to_PC_Local(message_1);
  String_to_PC_Local(message_2);
  String_to_PC_Local(message_3);
  String_to_PC_Local(message_4);
  while(switch_1_up)wdr();

 String_to_PC_Local("Print out of program memory\r\n\r\n");

mem_add = (char*)256;

for(int m = 0; m <=320; m++){
  if ((*(mem_add + m))== '\0')Long_Hex_and_Int_to_PC_Basic (10,256+m);
 else Char_to_PC_Basic(*(mem_add + m));}

  newline_Basic();newline_Basic();
  
  while(switch_1_down)wdr();
 while(switch_2_up)wdr();
  
  return 1;}