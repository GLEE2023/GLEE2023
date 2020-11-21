/******************************************************************************
  Example3_SetOffsetTemperatureValue.ino
  Example for the TMP117 I2C Temperature Sensor
  Madison Chodikov @ SparkFun Electronics
  May 29 2019
  ~

  This sketch configures the TMP117 temperature sensor and allows the user to
  set the offset temperature for System Correction.

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
*/

#include <Wire.h> // Used to establish serial communication on the I2C bus
#include <SparkFun_TMP117.h> // Used to send and recieve specific information from our sensor

// The default address of the device is 0x48 (GND)
TMP117 sensor; // Initalize sensor


void setup()
{
  Wire.begin();
  Serial.begin(115200);    // Start serial communication at 115200 baud
  Wire.setClock(400000);   // Set clock speed to be the fastest for better communication (fast mode)

  Serial.println("TMP117 Example 3: Set Temperature Offset Value");
  if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
  else
  {
    Serial.println("Device failed to setup- Freezing code.");
    while (1);
  }

  Serial.println(); // Create a new line for the loop for easier readings
  Serial.print("Current Temperature Offset (in °C): ");
  Serial.println(sensor.getTemperatureOffset());
}


// For function to work properly, make sure the Serial Monitor is set to "No Line Ending"
// The temperature offset function will not work if set outside the sensors bounds of +/- 255.98°C
void loop()
{
  float tempOffset = 0;
  if (sensor.dataReady() == true)  // Only prints when there is data ready from the temperature sensor
  {
    Serial.print("Enter new temperature offset (in °C): ");
    while (Serial.available() == 0); // Waits for the user input
    tempOffset = Serial.parseFloat(); // Reads the input from the serial port, adds 1 for precision
    Serial.println(tempOffset);
    if (tempOffset > 255.98 || tempOffset < -255.98)
    {
      Serial.println("Please enter a number within the range of +/-255.98°C");
      Serial.println();
    }
    else
    {
      sensor.setTemperatureOffset(tempOffset); // Write to the temperature offset register
      delay(1000); // Delay for conversion to successfully work
      Serial.println(); // Create a new line after each run of the loop
      Serial.print("New Offset Temperature: ");
      Serial.print(sensor.getTemperatureOffset()); // Print the new temperature offset
      Serial.println("°C");
      Serial.print("Temperature with Offset: ");
      Serial.print(sensor.readTempC());
      Serial.println("°C");
    }
  }
}
