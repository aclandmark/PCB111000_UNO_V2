



int main (void)
{  setup_HW;

if(watch_dog_reset != 1)String_to_PC_Basic(message_1);
  
  sei();
  T1_clock_tick(37);

  while (1) {
    Num_string_from_KBD_Local(Num_string);
    Num_string_to_PC_Basic(Num_string);
    Char_to_PC_Basic('\t');
   while(!(ready_to_compute));                    //Wait here for best time to start computation
   ready_to_compute = 0;
   num_present = 0; 
   Num = Askii_to_binary_Local(Num_string);       //Convert number entered at the KBD to binary
    root_computation(Num, Num_string);            //Calculate the root, and save in "Num_string" (see Int_to_String_Basic() in Localsubroutines)
    Num_string_to_PC_Basic(Num_string);           //Send result to the PC
    newline_Basic();
    Char_to_PC_Basic('?'); }}
