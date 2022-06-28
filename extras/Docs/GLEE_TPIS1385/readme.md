# GLEE - LunaSat Infrared (IR) Thermopile Sensor - GLEE_TPIS1385

This library is intended for GLEE's Thermopile Sensor, the TPIS1385. Paired with the GLEE's LunaSat object class, it can be used to monitor, configure and control the TPIS1385. GLEE_TPIS1385 supports basic functionality such as object temperature observations.

![High Precision Thermopile Sensor - TPIS1385](/Docs/Images/TPIS1385_close_up.jpg )

## Datasheet
* [High Precision Thermopile Sensor Datasheet- TPIS1385](https://media.digikey.com/pdf/Data%20Sheets/Excelitas%20PDFs/TPiS_1S_1385.pdf)



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

## Registers

| Register Name | Register Number | Size (bits) |
|---|---|---|
| reserved | 0 | 8 |
| TP_object | 1-2,3[7] | 17 |
| TP_ambient | 3[6:0],4 | 15 |
| TP_ObjLP1 | 5-7[7:4] | 20 |
| TP_ObjLP2 | 7[3:0]-9 | 20 |
| TP_ambLP3 | 10-11 | 16 |
| TP_ObjLP2 frozen | 12-14 | 24 |
| TP_presence | 15 | 8 |
| TP_motion | 16 | 8 |
| TP_amb shock | 17 | 8 |
| Interrupt Status | 18[7:0] | 8 |
| Chip Status | 19[7:0] | 8 |
| S_LP1 | 20[3:0] | 4 |
| S_LP2 | 20[7:4] | 4 |
| S_LP3 | 21[3:0] | 4 |
| reserved | 21[7:4] | 4 |
| TP_presence threshold | 22 | 8 |
| TP_motion threshold | 23 | 8 |
| TP_amb shock threshold | 24 | 8 |
| Interrupt Mask Register | 25[4:0] | 5 |
| reserved | 25[7:5] | 3 |
| Cycle time for Motion differentiation | 26[1:0] | 2 |
| SRC select for presence determination | 26[3:2] | 2 |
| TP_OT direction | 26[4] | 1 |
| reserved | 26[7:5] | 3 |
| Timer interrupt | 27[7:0] | 8 |
| TP_OT threshold | 28,29 | 16 |
| reserved | 30 | 8 |
| EEPROM control | 31 | 8 |
| EEPROM content | 62:32 | 248 |
| Slave address | 63 | 8 |

## Methods 
| Method Name | return type | args | Comments |
|---|---|---|---|
| begin | void | void | Begin comunications with TPIS1385 |
| readEEprom | void | void | |
| getTPamb | uint16_t | void | |
| getTamb | float | uint16_t | |
| getTPobj | uint32_t | void | |
| getTobj | float | uint32_t , float | |
| getCorrectedTobj | float | uint32_t , float , float | |
| getSample | TPsample_t | void | |
| getCorrectedSample | TPsample_t | float | |
| updateSample | void | void | |