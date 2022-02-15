Motor Input/Outputs:

| Connection  |      Purpose|
|----------|:-------------:|
| Pin 1 | V_hall |
| Pin 2 | Hall Sensor 3 | 
| Pin 3 | Hall Sensor 1 |
| Pin 4 | Hall Sensor 2 |
| Pin 5 | GND | 
| Pin 6 | Motor Winding 3 |
| Pin 7 | Motor Winding 2 |
| Pin 8 | Motor Winding 1 |


Motor Controller Interface:
TODO: Add Arduino connections for enable, direction and ready 
| Motor Pin  | Controller Pin | Arduino (Due) | Description |
|:----------:|:-------------:|:----------:|:--------:|
| 1 | 6 | - | V_hall (5V) |
| 2 | 9 | - | Hall 3 |
| 3 | 7 | - | Hall 1 |
| 4 | 6 | - | Hall 2 |
| 5 | 5 | - | GND |
| 6 | 3 | - | Winding 3 |
| 7 | 2 | - | Winding 2 |
| 8 | 1 | - | Winding 1 |
| - | 4 | - | Supply V_cc |
| - | 10 | Pin 8 | Ready Status |
| - | 11 | Pin 2 [HIGH] | DigIN1| 
| - | 12 | GND |DigIN2 |
| - | 13 | Pin 4 | Enable [active high] |
| - | 17 | Pin 6 [PB 15] | Speed Control |
| - | GND | GND | Ground|


## Controller Pin Refrence

![Motor Controller Pinouts](https://github.com/GLEE2023/GLEE2023/blob/dev/extras/Docs/Images/Arduino_Due_Refrence.JPG)

## Motor Pin Refrence

![Motor Controller Pinouts](https://github.com/GLEE2023/GLEE2023/blob/dev/extras/Docs/Images/Motor-Pin-Refrence.JPG)

## Arduino Pin Refrence

![Arduino Pinout](https://github.com/GLEE2023/GLEE2023/blob/dev/extras/Docs/Images/Motor_Refrence.JPG)
