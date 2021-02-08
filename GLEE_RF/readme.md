# GLEE RF
This library is intended for GLEE's RF (radio frequency). 
It is used to facilitate communication between LunaSats and the lander, where
wireless communication will be used to transmit data from the LunaSat to the 
central hub on the host lander with RF.

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

## Configuration Details
| Configuration Name | Default Setting |
|---|---|
| Freqeuncy | 915.0 MGh |
| Power | 17 |
| Bandwith | 250 |
| Spreading Factor | 12 |
| Coding Rate | 8 |
| NSS_PIN | 10 |
| DIO1_PIN | 3 |
| DIO0_PIN | 2 |
| RESET_PIN | 9 |

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
  Serial.begin(115200);
  Rad.initialize_radio();
}

void loop() {
  Rad.transmit_data("Hello World!");
  delay(1000);  
}
```