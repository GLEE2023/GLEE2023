# GLEE_TPIS1385

This library is intended for GLEE's Thermopile Sensor, the TPIS1385. Paired with the GLEE's LunaSat object class, it can be used to monitor, configure and control the TPIS1385. GLEE_TPIS1385 supports basic functionality such as object temperature observations.

![High Precision Thermopile Sensor - TPIS1385](/Docs/Images/TPIS1385_close_up.jpg)

## Example
* GLEE_TPIS1385
  * Thermopile sensor - Object temperature observations
  ```C++
    #include "TPIS1385.h"

    TPIS1385 thermopile(1);

    TPsample_t temperatures;

    void setup(){
        Serial.begin(9600);
        thermopile.begin();
        thermopile.readEEprom(); 
    }

    void loop(){
        temperatures = thermopile.getSample();
        Serial.print("Object temperature (F): ");      
        Serial.println(temperatures.object);
        Serial.print("Ambient temperature (F): "); 
        Serial.println(temperatures.ambient);
        delay(1000);
    }
    ```
