Motor Input/Outputs:

| Connection  |      Purpose|
|----------|:-------------:|
| Pin 1 | V_hall |
| Pin 2 | Hall Sensor 3 | 
| Pin 3 | Hall Sensor 2 |
| Pin 4 | Hall Sensor 1 |
| Pin 5 | GND | 
| Pin 6 | Motor Winding 3 |
| Pin 7 | Motor Winding 2 |
| Pin 8 | Motor Winding 1 |

Controller Input/Output:

| Connection  |      Purpose|
|----------|-------------:|
| Set Speed | Analogue Input 0-5V |
| Enable | Hall Sensor 3 | 
| Direction | Hall Sensor 2 |
| Speed Range DigIn1 | Hall Sensor 1 |
| Speed Range DigIn2 | GND | 
| Set Current Limit | External Resistor |
| Hall Sensor | Hall 1, 2, 3 |
| Motor | Winding 1, 2, 3 |

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
| - | 17 | PB 15 | Speed Control |
| - | GND | GND | Ground|
| - | 11 | HIGH | DigIN1| 
| - | 12 | LOW |DigIN2 |

## Controller Pin Refrence

![Motor Controller Pinouts](https://github.com/GLEE2023/GLEE2023/blob/dev/extras/Docs/Images/Arduino_Due_Refrence.JPG)

## Motor Pin Refrence

![Motor Controller Pinouts](https://github.com/GLEE2023/GLEE2023/blob/dev/extras/Docs/Images/Motor-Pin-Refrence.JPG)

## Arduino Pin Refrence

![Arduino Pinout](https://github.com/GLEE2023/GLEE2023/blob/dev/extras/Docs/Images/Motor_Refrence.JPG)
