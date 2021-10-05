// Sensor Includes
#include "TMP117.h"
#include "MLX90393.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"
#include <GLEE_Radio.h>

// Sensor Initializations
TMP117 thermometer(1, false);
MPU6000 accelerometer(2, false);
MLX90393 magnetometer(3, false);
TPIS1385 thermopile(4, false);
CAP capacitive(5, false);

// Sensor Data Variable Initialization
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;

//Initialize RF Object
LunaRadio Rad;

String printOut;
String printOut2;
String sep = ",";
int timeStamp;

int mode = 1;
int id = 1; // Change this for each lunasat

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

void setup() {
  // put your setup code here, to run once:
  Wire.begin();           // Begin Serial Communications
  Wire.setClock(100000);  // Set Standard 100Mhz i2c speed
  Serial.begin(9600);
  
  /* I2C Code Here */
  
  if(!Serial){
    delay(1000); // Delay if issue with serial
  }

  Serial.println(F("LunaSat Connection Succsessful"));

  // Initalize Thermopile Sensor
  Wire.beginTransmission(0x00);     // Tx buffer
  Wire.write(0x04);                 // Add Register Address to Tx buffer
  Wire.write(0x00);                 // Add initialization data to Tx buffer
  Wire.endTransmission();           // Send

  delay(50);

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
  
    Serial.print(F("Found: "));
    Serial.println(numDevices);

    delay(1000);



    // No begin method for TMP117
    
    accelerometer.begin(); // Begins transmission to the I2C slave device
    accelerometer.initialize(); // Set-up for MPU 
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelrometer
    
    // Mag Begin
    magnetometer.begin_I2C();
   
    // Set gain
    magnetometer.setGain(MLX90393_GAIN_2_5X);
       
    // Set resolution
    magnetometer.setResolution(MLX90393_X, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16);
    
    // Set oversampling
    magnetometer.setOversampling(MLX90393_OSR_2);
    
    // Set digital filtering
    magnetometer.setFilter(MLX90393_FILTER_6);
    
    capacitive.begin();
    
    thermopile.begin();
    thermopile.readEEprom();
    
    Serial.println(F("Sensor Initi\n"));

    Rad.initialize_radio(915.0,17,250.0,12,8);
    
    Serial.println(F("Rad Init"));
    
    Serial.println(F("Sample Key: "));
    Serial.println(F("TMP117, MPU6000, MLX90393, CAP, TPIS1385"));
}

void loop() {
  
  if (Serial.available() > 0) {
      byte input = Serial.read();
      
      if(input == '1'){
          mode = 1;
      }else if(input == '2'){
          mode = 2;
      }else if(input == '3'){
          mode = 3;
      }
  }
  
  if(mode == 1){
      sensor_test_mode();
  }else if(mode == 2){
      transmit_mode();
  }else if(mode == 3){
      recieve_mode();
  }
}

void sensor_test_mode(){
    // Get Samples + Data
    timeStamp = millis();
   
    tempSample = thermometer.getTemperatureC();
    accSample = accelerometer.getSample();
    magSample = magnetometer.getSample();
    thermSample = thermopile.getSample();
    capSample = capacitive.getRawData();
   
    printOut = String(timeStamp) + " : TMP: "+
                String(tempSample) + " : MPU: " +
                String(accSample.x) + sep + String(accSample.y) + sep + String(accSample.z);
    printOut2 = " MLX: " +
                String(magSample.magnetic.x) + sep + String(magSample.magnetic.y) + sep + String(magSample.magnetic.z) + " : TPIS: " +
                String(thermSample.object,5) + sep + String(thermSample.ambient,5) + " : CAP: " +
                String(capSample);
   
    Serial.print(printOut);
    Serial.println(printOut2);
   
    delay(500);
}

void recieve_mode(){
  String output = Rad.receive_data_string();
  
	//Output the results 
	Serial.print(F("Recieved: ")); Serial.println(output);
	
	// Print recieved signal strength indicator
	Serial.print(F("RSSI: ")); Serial.println(Rad.getRSSI());
}

void transmit_mode(){
  String msg = String("Msg from" + String(id));
  
  // Length (with one extra character for the null terminator)
  int msg_len = msg.length() + 1; 
  
  // Prepare the character array (the buffer) 
  char buff[msg_len];
  
  // Copy it over 
  msg.toCharArray(buff, msg_len);
  Rad.transmit_data(*buff); 
	delay(1000); // Send the transmission once every ~1 sec
}