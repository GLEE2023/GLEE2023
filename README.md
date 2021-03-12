# Great Lunar Expedition For Everyone 2023 Library Collection

The Great Lunar Expedition for Everyone mission will provide accessible opportunities for students to directly participate in Lunar exploration. GLEE will educate, inspire and enable the next generation of explorers with its easy to use LunaSat platform. GLEE's LunaSats, post-it note size wireless enabled sensing nodes, offer a range of functionalities relevant to both Lunar and Earth based scientific investigations. GLEE believes that providing students with LunasSats and associated education content will foster deep learning of many STEM fundamentals. Furthermore, challenging students to identify investigations which utilize standalone and networked LunaSats will foster novel crowdsourced approaches to Lunar exploration while also promoting innovations beneficial to life on Earth. The major goal of GLEE is to deliver and disperse 500 student team owned LunaSats as part of a distributed environmental sensing network on the surface of the moon. The prototyping of LunaSat and creation of educational modules is funded by NASA's Artemis Student Challenge initiative. 

GLEE's current prototype LunaSat is approximately 5cm by 5cm, has a mass of about 5 grams, is solar powered, controlled via an ATMEGA328P and uses LoRA to wirelessly transmit environmental data. The sensing suite consists of an accelerometer, gyroscope, temperature sensor, thermopile and a magnetometer. Camera and pin diode modules are also currently in development. 

This repo contains the Arduino libraries and sketches wich are used to control the lunasats. 


# Installation Instructions
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
  * Lunasat temperature sensing functionality

## LunaSat Classes
* LunaSat: 
  * Convient functionality for lunasat data and sensor interfacing standardization. Usefull for testing and abstracting lunasat functionality

* GLEE_RF: 

# README TODOS
* TODO: Installation Instructions
* TODO: Example pointouts
* TODO: Add contributor Names
* TODO: Embedded links to libraries
* TODO: GLEE_RF library instructions

# Development TODOS
* TODO: Iron out RF library dependencies/file structuring/ installation 
