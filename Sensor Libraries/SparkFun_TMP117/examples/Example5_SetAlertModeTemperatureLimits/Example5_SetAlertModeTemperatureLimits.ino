/******************************************************************************
  Example5_SetAlertModeTemperatureLimits.ino
  Example for the TMP117 I2C Temperature Sensor
  Madison Chodikov @ SparkFun Electronics
  June 11 2019
  ~

  
  This sketch can set and get the Alert Function Mode, Low Temperature 
  Limit, and High Temperature Limit for the sensor. These limits can
  be set within +/- 256°C. When the temperature goes above/below the 
  specified temperature limits, it will cause the alert pins to go
  high. To access these registers, please reference Example 2: Alert Statuses.

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
  linked on Page 35 of the TMP117's datasheet.
*/

#include <Wire.h> // Used to establish serial communication on the I2C bus
#include <SparkFun_TMP117.h> // Used to send and recieve specific information from the sensor

// The default address of the device is 0x48 (GND)
TMP117 sensor; // Initalize sensor


void setup()
{
  Wire.begin();
  Serial.begin(115200);    // Start serial communication at 115200 baud
  Wire.setClock(400000);   // Set clock speed to be the fastest for better communication (fast mode)

  Serial.println("TMP117 Example 5: Setting Alert Function Mode and Temperature Limits");
  if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
  else
  {
    Serial.println("Device failed to setup- Freezing code.");
    while (1); // Runs forever if the sensor does not initialize correctly
  }

  Serial.print("Current Alert Function Mode: ");
  Serial.println(sensor.getAlertFunctionMode());
  Serial.print("Current Low Limit: ");
  Serial.println(sensor.getLowLimit()); // Returns the low limit temperature in °C
  Serial.print("Current High Limit: ");
  Serial.println(sensor.getHighLimit()); // Returns the high limit temperature in °C
  Serial.println(" ");
}


// For function to work, make sure the Serial Monitor is set to "No Line Ending"
void loop()
{
  float lowTemp, highTemp;
  int alertMode;
  Serial.println("Enter a value to change the alert function mode, low limit, or high limit.");
  Serial.println("0 for Alert Function Mode, 1 for Low Limit, and 2 for High Limit: ");
  
  while (Serial.available() == 0); // Waits for the user input
  int menu_option = Serial.parseInt(); // Reads the input from the serial port

  if (menu_option == 0)
  {
    Serial.print("Current Alert Function Mode: ");
    Serial.println(sensor.getAlertFunctionMode());
    Serial.println("Please enter mode(0 or 1): ");
    while (Serial.available() == 0); // Waits for the user input
    alertMode = Serial.parseFloat();
    if (alertMode == 0)
    {
      sensor.setAlertFunctionMode(0);//set to alert mode
      Serial.print("New Alert Function Mode: ");
      Serial.println(sensor.getAlertFunctionMode());
    }
    else if (alertMode == 1)
    {
      sensor.setAlertFunctionMode(1);//set to therm mode
      Serial.print("New Alert Function Mode: ");
      Serial.println(sensor.getAlertFunctionMode());
    }
    else
    {
      Serial.println("Please enter the Alert Function Mode (either 0 or 1)");
    }
  }
  else if (menu_option == 1)
  {
    Serial.print("Current Low Limit: ");
    Serial.println(sensor.getLowLimit());
    Serial.println("Please enter Low Limit Temperature (between -256°C and 255.98°C): ");
    while (Serial.available() == 0); // Waits for the user input
    lowTemp = Serial.parseFloat();
    if ((lowTemp >= -256) && (lowTemp < 255.99))
    {
      sensor.setLowLimit(lowTemp);
      Serial.print("New Low Limit (in °C): ");
      Serial.println(sensor.getLowLimit());
    }
    else
    {
      Serial.println("Please enter a temperature between -256°C and 255.98°C");
    }

  }
  else if (menu_option == 2)
  {
    Serial.print("Current High Limit: ");
    Serial.println(sensor.getHighLimit());
    Serial.println("Please enter High Limit Temperature (between -256°C and 255.98°C): ");
    while (Serial.available() == 0); // Waits for the user input
    highTemp = Serial.parseFloat(); // Reads the input from the serial port
    if ((highTemp >= -256) && (highTemp < 255.99))
    {
      sensor.setHighLimit(highTemp);
      Serial.print("New High Limit (in °C): ");
      Serial.println(sensor.getHighLimit());
    }
    else
    {
      Serial.println("Please enter a temperature between -256°C and 255.98°C");
    }
  }
  else
  {
    Serial.println("Please enter 0, 1, or 2");
  }
  Serial.println(""); // Create a whitespace for easier readings
}

