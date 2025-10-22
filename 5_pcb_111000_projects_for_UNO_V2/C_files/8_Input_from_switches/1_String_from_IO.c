


int main (void){
long Num_1;

setup_HW_A;

sei();
while(switch_1_down) ;                                             //wait for switch release

set_up_pci;
enable_pci;

Init_display_for_pci_data_entry;                                    //Set digit[0] to zero and display it.
while(!(Data_Entry_complete));                                     //Line A. wait here for pci interrupts used to enter data
Data_Entry_complete = 0;
disable_pci;
Num_1 =  I2C_displayToNum();                                         //Acquire binary value of the display and return it.

rotate_display();

while(switch_1_up);
SW_reset;}
