/******************************************************************************
  Example7_AdvancedI2CFunctions.ino
  Example for the TMP117 I2C Temperature Sensor
  Madison Chodikov @ SparkFun Electronics
  July 22 2019
  ~

  This sketch allows the user to change the address of the device and to
  change the Wire port for I2C Communications. The address can be physically
  changed with an external jumper on the back of the sensor.

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

#include <Wire.h>            // Used to establish serial communication on the I2C bus
#include <SparkFun_TMP117.h> // Used to send and recieve specific information from our sensor

TMP117 sensor; // Initalize sensor

/* The default address of the device is 0x48 (GND)
 Sensor address can be changed with an external jumper to:
 VCC = 0x49
 SDA = 0x4A
 SCL = 0x4B
 For a table of the addresses, reference page 19, Table 2 on the TMP117 Datasheet 

 
 This sketch only works if your platform doesn't have multiple I2C ports. This will not 
 work on an Arduino Uno platform.
 
 To test the functionality of changing the addresses, change "Wire1" to "Wire" below
 */

void setup()
{
  Wire1.begin(); // Compilation will fail here if your platform doesn't have multiple I2C ports
  Serial.begin(115200);    // Start serial communication at 115200 baud
  Wire1.setClock(400000);   // Set clock speed to be the fastest for better communication (fast mode)

  Serial.println("TMP117 Example 7: Advanced I2C Functions");
  if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
  else
  {
    Serial.println("Device failed to setup- Freezing code.");
    while (1); // Runs forever
  }
  Serial.println("Device Address Options: ");
  Serial.println("1: 0x48 (GND/Default)");
  Serial.println("2: 0x49 (V+)");
  Serial.println("3: 0x4A (SDA)");
  Serial.println("4: 0x4B (SCL)");
  Serial.println(); // White space for easier readings
  Serial.print("Current Device Address: ");
  Serial.println(sensor.getAddress(), HEX); // Prints the current address of the device in Hex
}

void loop()
{
  Serial.println("Enter new address (number 1 - 4): ");
  while (Serial.available() == 0); // Waits for the user input
  uint8_t newAddress = Serial.parseInt(); // Reads the input from the serial port
  Serial.print("Number received: ");
  Serial.println(newAddress);
  if(newAddress == 1)  // 0x48 (GND/Default)
  {
    sensor.begin(0x48, Wire);
    Serial.print("New Address: ");
    Serial.println(sensor.getAddress(), HEX);
    Serial.println();
  }
  else if(newAddress == 2) // 0x49 (V+)
  {
    sensor.begin(0x49, Wire);
    Serial.print("New Address: ");
    Serial.println(sensor.getAddress(), HEX);
    Serial.println();
  }
  else if(newAddress == 3) // 0x4A (SDA)
  {
    sensor.begin(0x4A, Wire);
    Serial.print("New Address: ");
    Serial.println(sensor.getAddress(), HEX);
    Serial.println();
  }
  else if(newAddress == 4) // 0x4B (SCL)
  {
    sensor.begin(0x4B, Wire);
    Serial.print("New Address: ");
    Serial.println(sensor.getAddress(), HEX);
    Serial.println();
  }
  else // Runs when a number not 1-4 is entered
  {
    Serial.println("Please enter a number 1-4");
    Serial.println();
  }
}
