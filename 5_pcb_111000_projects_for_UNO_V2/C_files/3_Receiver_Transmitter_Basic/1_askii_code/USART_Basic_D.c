

int main (void)                          //Example 7
  { char symbol = '!';					//Print out askii table
  setup_HW_basic;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  newline_Basic();
  while (symbol <= '~')
  { Int_to_PC_Basic(symbol);
    Char_to_PC_Local(symbol++);
    wdr();_delay_ms(50);
    if (!((symbol - '!') % 8))newline_Basic();
    else Char_to_PC_Local('\t');
  }
  while(switch_1_up)wdr();
  while(switch_1_down);  }
  
  
  
  
int main (void)   			     //Example 8
  { setup_HW_basic;					//Lower to upper case conversion
  char keypress;
 char text[25];
 int m=0;
 
  if (watch_dog_reset == 1) String_to_PC_Local("\r\n?  ");
  else {User_prompt_Basic;String_to_PC_Local("\r\nEnter some text\
  maybe the name of a song then -cr-\r\n");}
  
  while (1)
  { keypress = waitforkeypress_Local();
    if  ((keypress =='\r') ||  (keypress =='\n'))break;
    else {Char_to_PC_Local(keypress); text[m++] = keypress; }}
text [m] = '\0';
m=0;
while(text[m]){if((text[m]>='a') && (text[m]<= 'z'))
text[m] -= 32; m+=1;}
newline_Basic(); String_to_PC_Local(text);
SW_reset; }