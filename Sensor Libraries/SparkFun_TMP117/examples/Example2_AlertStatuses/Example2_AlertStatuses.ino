/******************************************************************************
  Example2_AlertStatuses.ino
  Example for the TMP117 I2C Temperature Sensor
  Modified by: Ho Yun "Bobby" Chan @ SparkFun Electronics
  Date Modified: November 12, 2019
  Written by: Madison Chodikov @ SparkFun Electronics
  Date: May 29 2019
  ~

  This sketch sets the TMP117 temperature sensor's high limit,
  low limit, and alert function mode. Once set, we read the 
  temperature in °C and checks alert status. If we are outside
  of the boundary, we will output a message indicating
  that we are beyond the limit. The alert statuses is dependent
  on the mode so make sure to 
  

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

byte AlertFlag = 0; //variable to hold high and low alert flags from configuration register
boolean H_AlertFlag = 0;  //variable to hold state of high alert flag
boolean L_AlertFlag = 0;  //variable to hold state of low alert flag

void setup()
{
  Wire.begin();
  Serial.begin(115200);    // Start serial communication at 115200 baud
  Wire.setClock(400000);   // Set clock speed to be the fastest for better communication (fast mode)
  Serial.println("TMP117 Example 2: Alert Statuses");
  if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
  else
  {
    Serial.println("Device failed to setup- Freezing code.");
    while (1);
  }

  Serial.println("");
  Serial.println("Note: Make sure to configure the your High and");
  Serial.println("Low Temperature Limits. These values will");
  Serial.println("be cleared on power cycle since it is only");
  Serial.println("saved in the volatile registers. This code");
  Serial.println("sets it manually. You can look at Example 5");
  Serial.println("in the library for more ideas!");

  /*Note: Set the high and low limits. Make sure to set limits
    between -256°C and 255.99°C. For quick testing at room
    temperature that is about 20°C-25°C, we can use the heat
    from our hand or lightly breathing on the sensor. Adjust
    as necessary.*/
  sensor.setHighLimit(25.50); //set high limit to 25.50°C
  sensor.setLowLimit(25.00);  //set low limit to 25.00°C

  //Get High Temperature Limit
  Serial.println("");
  Serial.print("High Temperature Limit: ");
  Serial.print(sensor.getHighLimit());
  Serial.println("°C");

  //Get Low Temperature Limit
  Serial.print("Low Temperature Limit: ");
  Serial.print(sensor.getLowLimit());
  Serial.print("°C");
  Serial.println("");

  /*Note: Set to alert or therm mode. To change, simply adjust
    add or remove a `//` to the line.*/
  sensor.setAlertFunctionMode(0);//set to alert mode
  //sensor.setAlertFunctionMode(1);//set to therm mode

  /*Get "Alert Function Mode" Bit from configuration register
    Note: Depending on the mode, this affects the way HIGH and
    LOW Alert Fields behave in the Configuration Register. For more
    information, check out the following sections of the datasheet:
      7.4.4.1 Alert Mode (pg 14)
      7.4.4.2 Therm Mode (pg 16)
      7.6.1.2 Configuration Register (pg 26)
  */
  delay(500);//wait a little before grabbing current mode
  Serial.print("Alert Function Mode = ");
  Serial.println(sensor.getAlertFunctionMode());
  Serial.println("----------------------------------------"); //start checking temp and alert flags after this line
}

/* Alert statuses below for high or low temperature reading
   possibilities: High Alert = 256°C, Low Alert = -256°C*/
void loop()
{
  // Data Ready is a flag for the conversion modes - in continous conversion the dataReady flag should always be high
  if (sensor.dataReady() == true) // Function to make sure that there is data ready to be printed, only prints temperature values when data is ready
  {
    /*Note: If you are in Alert Mode (T/nA = 0), the high and low alert
      flags will clear whenever you read the configuration register. You
      can add a delay to to perform another temperature conversion to trigger the
      flags again. The delay depends on the conversion cycle time so make
      sure to adjust as necessary to check the if the flags are triggered.
    */
    Serial.println("");
    Serial.print("Current Temperature: ");
    Serial.print(sensor.readTempC());
    Serial.println("°C");
    /*Note: Add short delay before reading the again configuration register
      adjust this value as necessary based on your conversion cycle time.
      Default conversion time for AVG = 1 and CONV = 4 about 1 second. Therefore,
      a value of between 1-3 seconds should be sufficient.*/
    delay(1500);//wait a little before grabbing

    AlertFlag = sensor.getHighLowAlert(); //read the alert flags from the configuration register
    H_AlertFlag = bitRead(AlertFlag, 1); //grab the high alert field using bitwise operator and save current to H_AlertFlag
    L_AlertFlag = bitRead(AlertFlag, 0); //grab the low alert field using bitwise operator and save current L_AlertFlag

    //output byte and bits, Arduino will not output leading 0's toward the MSB
    Serial.print("High and Low Alert Flags = ");
    Serial.println(AlertFlag, BIN);
    Serial.print("High Alert Flag = ");
    Serial.println(H_AlertFlag, BIN);
    Serial.print("Low Alert Flag = ");
    Serial.println(L_AlertFlag, BIN);

    if (H_AlertFlag == true)
    {
      /*Alert when the temperature is over the HIGH limit:
         - In Alert Mode (T/nA = 0) this flag will clear
        when the configuration register is read.
         - In Therm mode (T/nA = 1), this flag will clear ONLY
         when we have reached the lower limit. This high and
         low limits act as a hystersis.
      */
      Serial.println("High Alert");
    }
    else if (L_AlertFlag == true)
    {
      /*Alert when the temperature is over the LOW limit:
         - In Alert Mode (T/nA = 0) this flag will clear
        when the configuration register is read.
         - In Therm mode (T/nA = 1), this flag is always 0
      */
      Serial.println("Low Alert");
    }
    else
    {
      Serial.println("No Alert");
    }
    delay(500); // Delay for a 1/2 second before printing again if the data is ready
  }
}
