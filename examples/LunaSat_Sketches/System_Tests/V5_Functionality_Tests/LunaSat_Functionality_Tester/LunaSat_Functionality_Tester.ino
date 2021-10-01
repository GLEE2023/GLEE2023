// Sensor Includes
#include "TMP117.h"
#include "MLX90393.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"
#include <Wire.h>
​
// Sensor Initializations
TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90393 magnetometer(3);
TPIS1385 thermopile(1);
CAP capacitive(5);
​
// Sensor Data Variable Initialization
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;
​
String printOut;
String sep = ",";
int timeStamp;

// Standard V4 Lunasat I2C Sensors
// AK09940 - Magnetometer
// TMP117 - Temperature Sensor
// TPIS1385 - Thermopile
// ICM20602 - Accelerometer

// Standard V4 Lunasat I2C Address Configuration (7-Bit Addresses)

// AK09940:   0x0C
// TMP117:    0x48
// TPIS1385:  0x0D
// ICM20602:  0x69

byte sensorsAddrs[4] = {0x0C, 0x48, 0x0D, 0x69};

String sensorNames[4] = {"Mag", "Temp", "Therm", "Accelerometer"};

byte addr; // Byte representation of I2C addresses
byte err; // Byte to store error responses
int numDevices; // Accumulator for tracking number of found devices
​
void setup() {
  // put your setup code here, to run once:
  Wire.begin();           // Begin Serial Communications
  Wire.setClock(100000);  // Set Standard 100Mhz i2c speed
  Serial.begin(9600);
​
  /* I2C Code Here */

  if(!Serial){
    delay(1000); // Delay if issue with serial
  }

  Serial.println("LunaSat Connection Succsessful");

  // Initalize Thermopile Sensor
  Wire.beginTransmission(0x00);     // Tx buffer
  Wire.write(0x04);                 // Add Register Address to Tx buffer
  Wire.write(0x00);                 // Add initialization data to Tx buffer
  Wire.endTransmission();           // Send

  delay(50);

  Serial.println("Sensor initialization Initialization Succsessful\n");

  pinMode(A3,OUTPUT); // TODO: Remove if depricated
  digitalWrite(A3,HIGH);

  byte addr;          // Current address for scanning
  int numDevices = 0; // Number of devices found

  // Loop through expected addresses of sensors
  for (int i=0; i<4; i++){

    addr = sensorsAddrs[i];
    Wire.beginTransmission(addr);
    err = Wire.endTransmission();
    
    if(!err){
      numDevices++;
      Serial.print(sensorNames[i]);
      Serial.print(" sensor found at ");
      
      // Hex address formating "0x00"
      Serial.print("0");
      Serial.print(addr,HEX);
      Serial.println("  !");
      
    }else if(err==4){
      Serial.print("Error at ");
      Serial.print("0");
      Serial.println(addr, HEX);
    }
  }
  
  Serial.print("Scan Complete: ");
  Serial.print(numDevices);
  Serial.println(" devices found\n");
  
  delay(1000);

  

  // No begin method for TMP117
​
  accelerometer.begin(); // Begins transmission to the I2C slave device
  accelerometer.initialize(); // Set-up for MPU 
  accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelrometer 
​
  // Mag Begin
  magnetometer.begin_I2C();
​
  // Set gain
  magnetometer.setGain(MLX90393_GAIN_2_5X);
​
  // Set resolution
  magnetometer.setResolution(MLX90393_X, MLX90393_RES_19);
  magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19);
  magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16);
​
  // Set oversampling
  magnetometer.setOversampling(MLX90393_OSR_2);
​
  // Set digital filtering
  magnetometer.setFilter(MLX90393_FILTER_6);
​
  capacitive.begin();
​
  thermopile.begin();
  thermopile.readEEprom();
​
  Serial.println("Sample Key: ");
  Serial.println("TMP117, MPU6000, MLX90393, CAP, TPIS1385");
​
}
​
void loop() {
​
  // Get Samples + Data
  timeStamp = millis();
​
  tempSample = thermometer.getTemperatureC();
  accSample = accelerometer.getSample();
  magSample = magnetometer.getSample();
  thermSample = thermopile.getSample();
  capSample = capacitive.getRawData();
​
  printOut = String(timeStamp) + " : TMP: "+
             String(tempSample) + " : MPU: " +
             String(accSample.x) + sep + String(accSample.y) + sep + String(accSample.z);
             String printout2 = " MLX: " +
             String(magSample.magnetic.x) + sep + String(magSample.magnetic.y) + sep + String(magSample.magnetic.z) + " : TPIS: " +
             String(thermSample.object,5) + sep + String(thermSample.ambient,5) + " : CAP: " +
             String(capSample);
​
  Serial.print(printOut);
  Serial.println(printout2);
​
  delay(500);
​
}