

int main (void)
  { char symbol;
    char q = 0;
    int n = 0;
    
  setup_HW;

  if (watch_dog_reset != 1)
newline_Basic();

    UCSR0B |= (1 << RXCIE0);
  sei();
newline_Basic();
  symbol = '!';
  while (symbol <= '~')
  { Char_to_PC_Basic(symbol);
    _delay_ms(75);
    symbol++;
    sym_counter++;
    wdr();
  }
  cli();waitforkeypress_Basic();
  
newline_Basic();
SW_reset;}
