



/*
A selection of short programs designed to show what a simple C program might look like
and give practice entering and compiling several of them.

It introduces:
one_wire subroutines which send data to the mini_OS that resides on the ATtiny1606
and drives the display.
 
The setup_328_HW_Arduino_IO: This sets up the IO, the watch dog timer, calibrates the 8MHz clock etc...
The SW_reset macro: This enables the Atmega328 to be reset at any time.

The purpose of the watch dog timer (WDT) is to reset the program if it crashes 
wdr(): This is needed to reset the watchdog timer and prevent it from timing out and resetting the program unnecessarily. 

A PRN generator that produces pseudo random numbers.
*/



#include "First_project_header_1.h"
#include "First_project_header_2.h"




  
/***************************************************************************************************/
