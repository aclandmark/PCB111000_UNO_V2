# PCB111000_UNO_V2
PCB111000_1 was a version of PCB11100 designed to plug in to a UNO rather than be a stand-a-lone PCB.
System code for PCB111000_1 otherwise known as PCB111000_UNO was developed using winAVR.

I believe that winAVR has largely been replaced by Atmel Studio and Arduino.
PCB111000_UNO is therefore being replaced by PCB111000_UNO_V2 in which all winAVR project work is being
transferred to these compilers. Small changes will be introduced at the same time. For examle:
The rando number generator writes to EEPROM far to frequently resulting in EEPROM burn-out. 
This will be ofset so that burn out takes far longer to occur.

In addition sample user projects developed for PCB111000_UNO will be made compatible with those developed for PCB111000_CP2102.
The UNO projects will use the I2C bus to trandfer data to the display while the CP2102 projects will use a firmware one wire UART link.

