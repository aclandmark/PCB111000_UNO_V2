

int main (void)
  { char symbol;
    char q = 0;
    int n = 0;
    
  setup_HW;

    UCSR0B |= (1 << RXCIE0);
  sei();

  do {
  symbol = 0;
  while (symbol <= '~' - '!')
  { Char_to_PC_Basic((symbol+q)%94 + '!');
    _delay_ms(5);
    symbol++;}
    _delay_ms(50);
q += 1;
newline_Basic();
}while(repeats[n++]);
 cli();waitforkeypress_Basic();
newline_Basic();
SW_reset;}

