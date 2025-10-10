


int main (void)   
  {char keypress;
  setup_HW;

  sei();
  T1_clock_tick(100);

if(watch_dog_reset != 1)String_to_PC_Basic("Send file: Ctrl-Z when finished\r\n");
  
while (!(isCharavailable_Basic(65)))
    Char_to_PC_Basic('?');  
  newline_Basic();
Char_to_PC_Basic(Char_from_PC_Basic());

while ((keypress = waitforkeypress_Basic()) != ctlz)Char_to_PC_Basic(keypress);
 String_to_PC_Basic("\r\nDone: AK to repeat\r\n");
SW_reset;
    }