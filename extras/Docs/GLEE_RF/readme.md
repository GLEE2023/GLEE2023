# GLEE RF
This library is intended for GLEE's RF (radio frequency) transmitter module (SX1272). 
It is used to facilitate communication between LunaSats and the lander, where
wireless communication will be used to transmit data from the LunaSat to the 
central hub on the host lander with RF.

This library is a port of Radiolib. Make sure to install radiolib via Arduino Libraries manager before using this library. (Huge thank you to Jgromes for that amazing lora library)

PCB antenna and SX1272 pictured below\
![PCB Antenna](/extras/Docs/Images/SX1272_antenna_close_up.jpg) ![SX1272 Chip](/extras/Docs/Images/SX1272_close_up.jpg) 

## RF Receive Example
```C++
#include <GLEE_Radio.h>

LunaRadio Rad;

void setup() {
  Serial.begin(9600);
  Rad.initialize_radio(915.0,17,250.0,12,8);
}

void loop() {
  String output = Rad.receive_data_string();
  Serial.println(output);
}
```

## RF Transmit Example
```C++
#include <GLEE_Radio.h>

LunaRadio Rad;


void setup() {
  Serial.begin(9600);
  Rad.initialize_radio();
}

void loop() {
  Rad.transmit_data("Hello World!");
  delay(1000);  
}
```
## ATMEGA 328 + SX1272 Pin Configuration Details
| Configuration Name | Default Setting |
|---|---|
| NSS_PIN | 10 |
| DIO1_PIN | 3 |
| DIO0_PIN | 2 |
| RESET_PIN | 9 |

## Default Initialization
| Configuration Name | Default Setting |
|---|---|
| Freqeuncy | 915.0 MHz |
| Power | 17 |
| Bandwith | 250 |
| Spreading Factor | 12 |
| Coding Rate | 8 |

## Common Init Parameters

* freq

    FREQuency of radio waves received or transmitted by RF Module. Typically 915-928MHz corresponding to LoRas operation on unlicensed ISM Radio Band in US

* pwr

    PoWeR of transceiver transmission.  (TODO: Add refernce to relevent calculations given antenna parameters)

* bw
  
    BandWidth is the frequency range of chirps used to transmit data. Typically 250kHz for terrestrial usage in the US.

* sf

    Spreading Factor dictates the number of chirps used to send a signal.

* cr 

  Coding Rate effects data encoding redundancies. CR value is proportional to interference tolerance. (Higher - Able to handle more interference)



# Library Details

## Maximum Message Size 
256 Bytes 

## Methods 
| Method Name | return type | args | Comments |
|---|---|---|---|
| initialize_radio | void | float freq, uint8_t pwr, float bw, uint8_t sf, uint8_t cr | Initializes the radio |
| transmit_data | void | char* buff | Sends data as a char pointer |
| receive_data_string | string | none | Receives data as a string |
| receive_data_string_plotting | string | none | Receives data as a string and used for plotting |
| setFreq | void | float freq | Sets frequency |
| setCR | void | uint8_t cr | Sets coding rate |
| setSF | void | uint8_t sf | Sets spreading factor |
| setBandwith | void | float bw | Sets bandwith |
| setPWR | void | uint8_t pwr | Sets output power |

## External LoRA Documentation Refrences
[LoRA documentation Quick Refrence](https://lora.readthedocs.io/en/latest/#range-vs-power)
