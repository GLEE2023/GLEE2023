# Bootloading LunaSat V4

# Key Notes:
V4 uses Atmega328, not the Atmega328p, so the microprocessor has a hardware sig that is not handled in the avrdude configuration (avrdude.conf) 

# Requirements:
* Hardware
  * Arduino Uno
  * LunaSat
  * 5 x Jumper Wires
  
* Software:
  * Arduino IDE
  
# Wiring Configuration
* External Power to arduino when bootloading
TODO: Add wiring diagram alongside annotate image of bootload connections and jumper configuration

# Arduino IDE Hardware File Modification

## Avrdude Modifications
* Step 1: Naviagate to avrdude.conf and open file in text editor 
  * Windows: "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc"
  * Mac:
  
* Step 2: Locate default Atmega328p hardware adress configuration
  * Find: "0x1e 0x95 0x0F"
  * Replace with: "0x1e 0x95 0x14"
  * Save and Exit File
  
  * Note: After bootloading is complete, revert to default hardware value to avoid IDE configuration issues when using different boards (especially unos)
  
## ArduinoISP Sketch modification
* Step 1: Naviagate to avrdude.conf and open file in text editor 
  * Within the arduino ide, navigate to ArduinoISP sketch via File > Examples

* Step 2: Edit Sketch (save a backup to restore default IDE configuration after bootloading)
  * Within the Hearbeat function change change delay(40) to delay(20)
  * Save and Exit
  
## Configure Programmer Board
* Step 1: Open and Configure Arduino IDE
  * Tools > Board: set to Arduino Uno
  * Tools > Serial Port: set to correct usb coms port
  * Tools > Programmer: Arduino as ISP


  
