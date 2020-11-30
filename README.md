# GLEE 2023 Library Collection

# Instillation Instructions
Version Note: First draft/unverified instructions
1. Download Zipped GLEE2023 folder
2. Locate your arduino custom libraries folder 
 * Typically stored in ~Documents > Arduino > Libraries
 * Can also be found by navigating to add library from Arduino IDE
3. Extract the GLEE2023 folder into your custom libraries folder
4. Move the sketches folder to where you wish to keep your sketches

## Sensor Classes
* GLEE_Sensor: 
  * Parent class which provides standard sensor interfacing functionality for other sensor libraries

* GLEE_AK09940: 
  * Lunasat Magnetometer Functionality 

* GLEE_ICM20602: 
  * Lunasat Accelerometer functionality (plus additional IMU functionality)

* GLEE_Capacitive: 
  * LunaSat analog capacitance functionality

* GLEE_TMP117: 
  * Lunasat temnperature sensing functionality

## LunaSat Classes
* LunaSat: 
  * Convient functionality for lunasat data and sensor interfacing standardization. Usefull for testing and abstracting lunasat functionality
* GLEE_RF: 

# README TODOS
* TODO: Instellation Instructions
* TODO: Example pointouts
* TODO: Add contributor Names
* TODO: Emmbed links to libraries
* TODO: GLEE_RF library instructions

# Development TODOS
* TODO: Iron out RF library dependencies/file structuring/ instillation
