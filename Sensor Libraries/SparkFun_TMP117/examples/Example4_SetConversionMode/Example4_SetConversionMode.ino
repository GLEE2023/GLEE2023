/******************************************************************************
  Example4_SetConversionMode.ino
  Example for the TMP117 I2C Temperature Sensor
  Madison Chodikov @ SparkFun Electronics
  May 29 2019
  ~

  This sketch can get and set the conversion mode that the temperature sensor can be in,
  which is Continuous Conversion, Shutdown, or One-Shot. The specific values for these
  are found below at the end of the comments section.

  Resources:
  Wire.h (included with Arduino IDE)
  SparkFunTMP117.h (included in the src folder) http://librarymanager/All#SparkFun_TMP117

  Development environment specifics:
  Arduino 1.8.9+
  Hardware Version 1.0.0

  This code is beerware; if you see me (or any other SparkFun employee) at
  the local, and you've found our code helpful, please buy us a round!

  Distributed as-is; no warranty is given.
******************************************************************************/

/*
  NOTE: For the most accurate readings:
  - Avoid heavy bypass traffic on the I2C bus
  - Use the highest available communication speeds
  - Use the minimal supply voltage acceptable for the system
  - Place device horizontally and out of any airflow when storing
  For more information on reaching the most accurate readings from the sensor,
  reference the "Precise Temperature Measurements with TMP116" datasheet that is
  linked on Page 35 of the TMP117's datasheet

  The default address of the device is 0x48 (GND)

  There are 3 different modes
  Continuous Conversion (CC) = 0b00 = 1
  Shutdown (SD) = 0b01 = 2
  One-Shot Conversion (OS) = 0b11 = 3
*/

#include <Wire.h> // Used to establish serial communication on the I2C bus
#include <SparkFun_TMP117.h> // Used to send and recieve specific information from our sensor

// The default address of the device is 0x48 (GND)
TMP117 sensor; // Initalize sensor object

void setup()
{
  Wire.begin();
  Serial.begin(115200);    // Start serial communication at 115200 baud
  Wire.setClock(400000);   // Set clock speed to be the fastest for better communication (fast mode)

  Serial.println("TMP117 Example 4: Setting Conversion Modes");
  if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
  else
  {
    Serial.println("Device failed to setup- Freezing code.");
    while (1); // Runs forever if the sensor does not initialize correctly
  }

  Serial.println("Conversion Modes: ");
  Serial.println("1: Continuous");
  Serial.println("2: Shutdown");
  Serial.println("3: One-Shot");
  Serial.print("Current Conversion Mode: ");
  Serial.println(sensor.getConversionMode()); // Prints the conversion mode of the device to the Serial Monitor
}


// For function to work, make sure the Serial Monitor is set to "No Line Ending"
void loop()
{
  Serial.println("Enter your mode of Conversion (number 1 - 3): ");
  while (Serial.available() == 0); // Waits for the user input
  uint8_t convMode = Serial.parseInt(); // Reads the input from the serial port
  Serial.print("Number received: ");
  Serial.println(convMode);
  if (convMode == 1)
  {
    sensor.setContinuousConversionMode(); // Sets mode register value to be 0b00
    Serial.print("New Conversion Mode: ");
    if (sensor.getConversionMode() == 1) 
    {
      Serial.println("Continuous Conversion");
    }
    else
    {
      Serial.println("Error setting new Conversion Mode");
    }
    Serial.println(); // Create a whitespace for easier readings
  }
  else if (convMode == 2)
  {
    sensor.setShutdownMode(); // Sets mode register value to be 0b01
    Serial.print("New Conversion Mode: ");
    if (sensor.getConversionMode() == 2)
    {
      Serial.println("Shutdown Mode");
    }
    else
    {
      Serial.println("Error setting new Conversion Mode");
    }
    sensor.getConversionMode();
    Serial.println(); // Create a whitespace for easier readings
  }
  else if (convMode == 3)
  {
    sensor.setOneShotMode(); // Sets mode register value to be 0b11
    Serial.print("New Conversion Mode: ");
    if (sensor.getConversionMode() == 3)
    {
      Serial.println("One-Shot Mode");
    }
    else
    {
      Serial.println("Error setting new Conversion Mode");
    }
    sensor.getConversionMode();
    Serial.println(); // Create a whitespace for easier readings
  }
  else
  {
    Serial.println("Conversion mode unsuccessfully set - Please enter a number 1 - 3");
    Serial.println(); // Create a whitespace for easier readings
  }
  delay(1000);
}
