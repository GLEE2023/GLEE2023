# GLEE LunaSat Library
This library puts all the basic functionality of the lunasat in one place. The lunasat library can be included and utilized to conduct quick and easy initialization of subsystems and controlled querying and transmission of sensor observations.

## Sensors

* TMP117:
  * Digital Temperature Sensor
  
* AK09940:
  * 3-axis magnetometer
  
* ICM20602:
  * 3-axis Magnetometer
  * 3-axis gyroscope (currently unused)
  * Digital Temperature 
  
* Capacitive:
  * ADC Diaelectric Sensor
  
* Thermopile
  * Contactless IR thermal sensor
  
## Data Structures

### Lunasat Info
* ID: Lunasat identification number (helpfull when working with collections of lunasats)
* Name: String representing the name of the lunasat
* Sensor Config: Configurations parameters for sensors (TODO)
* RF Config: Wireless communication system configuration parameters (TODO)

### LunaSat Sample Types

### LunaSat Configuration
Default Configuration: {1,1,1,1,1}
Configuration Description: All sensors enabled and in default single measurement mode 

#Configuration Format
[0] - TMP117 
[1] - ICM20602
[2] - AK09940
[3] - TPiS1385
[4] - Radio
