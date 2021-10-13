// Sensor Includes
#include "TMP117.h"
#include "MLX90393.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"

// Sensor Initializations
TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90393 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

// Sensor Data Variable Initialization
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;

String printOut;
String sep = ", ";
int timeStamp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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

  Serial.println("Sample Key: ");
  Serial.println("Time, TMP117, MPU6000-X, MPU6000-Y, MPU6000-Z, MLX90393-X, MLX90393-Y, MLX90393-Z, TPIS1385-Object, TPIS1385-Ambient, CAP");

}

void loop() {

  // Get Samples + Data
  timeStamp = millis();

  tempSample = thermometer.getTemperatureC();
  accSample = accelerometer.getSample();
  magSample = magnetometer.getSample();
  thermSample = thermopile.getSample();
  capSample = capacitive.getRawData();

  printOut = String(timeStamp) + sep + String(tempSample) + sep + String(accSample.x) + sep + 
             String(accSample.y) + sep + String(accSample.z) + sep + String(magSample.magnetic.x) + sep + 
             String(magSample.magnetic.y) + sep + String(magSample.magnetic.z) + sep +
             String(thermSample.object,5) + sep + String(thermSample.ambient,5) + sep + String(capSample);

  Serial.println(printOut);

  delay(500);

}
