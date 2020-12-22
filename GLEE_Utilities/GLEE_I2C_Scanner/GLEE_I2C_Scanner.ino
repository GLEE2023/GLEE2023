// GLEE I2C Sensor 
// Customized I2C scanner for use in Verifying Lunasat Functionality and Troubleshooting

#include <Wire.h>

// Standard V4 Lunasat I2C Sensors
  // AK09940 - Magnetometer
  // TMP117 - Temperature Sensor
  // TPIS1385 - Thermopile
  // ICM20602 - Accelerometer

// Standard V4 Lunasat I2C Address Configuration (7-Bit Addresses)

  // AK09940:   0x0C
  // TMP117:    0x49
  // TPIS1385:  0x0D
  // ICM20602:  0x69

byte sensorsAddrs[4] = {0x0C, 0x49, 0x0D, 0x69};

String sensorNames[4] = {"Mag", "Temp", "Therm", "Accelerometer"};

byte addr; // Byte representation of I2C addresses
byte err; // Byte to store error responses
// int numDevices; // Accumulator for tracking number of found devices

void setup() {

  // Initialization
  Wire.begin();           // Begin Serial Communications
  Wire.setClock(100000);  // Set Standard 100Mhz i2c speed
  Serial.begin(9600);     // Set Default 

  if(!Serial){]
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
}

void loop() {

  byte addr;          // Current address for scanning
  int numDevices = 0; // Number of devices found

  // Loop through expected addresses of sensors
  for (i=0; i<4; i++){

    addr = addresses[i];
    Wire.beginTransmission(addr);
    err = Wire.endTransmission();
    if(!err){
      
      Serial.print(sensorNames[i]);
      Serial.print(" sensor found at ");
      // Hex address formating "0x00"

      Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

    }else if(err==4)
      Serial.print("Error at ");

      Serial.print("0");
      Serial.println(addr, HEX);
    }
  }

  delay(1000);
}
