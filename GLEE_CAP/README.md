# GLEE_CAP Sensor Library
This library is intended for GLEE's Capacitive Sensor. (pictured below)\
![Capacitive Sensor](/Docs/Images/CAP_close_up.jpg)

[Capacitive Sensor]()


## Examples
Capacitive Sensor Analog Read
```C++
#include "CAP.h"

Capacitive cap(1);

void setup() {
	Serial.begin(9600);
	cap.begin();
}

void loop(){
	int rawData = cap.getRawData(); 
	Serial.println(rawData);
};
```
