#include "MLX90393.h"

MLX90393 magnetometer = MLX90393(1,false);

mlx_sample_t sample;

float angle;
float magnitude;

float avgX = 0.0;
float avgY = 0.0;
float avgZ = 0.0;

String input = "";

void setup (){
    Serial.begin(9600);

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

    // Calibration
    Serial.println("Place your LunaSat at your desk with the magnet away.");
    Serial.println("Press Enter to calibrate your magnitometer.");

    while(!Serial.available()){}
    input = Serial.readString();

    mlx_sample_t calibrationPoints[20];    
    for(int i = 0; i < 20; i++){
      calibrationPoints[i] = magnetometer.getSample();
    }

    float avgX = 0.0;
    float avgY = 0.0;
    float avgZ = 0.0;

    for(int j = 0; j < 20; j++){
      avgX = avgX + calibrationPoints[j].magnetic.x;
      avgY = avgY + calibrationPoints[j].magnetic.y;
      avgZ = avgZ + calibrationPoints[j].magnetic.z;
    }

    avgX = avgX/20;
    avgY = avgY/20;
    avgZ = avgZ/20;

    Serial.println("Calibration is complete. You may now begin the activity.");
    Serial.println();
};

void loop (){
  Serial.println("Place your LunaSat above the point where you wish to draw a vector.");
  Serial.println("Press Enter to generate a vector.");
  
  // Wait for continue command from user (currently accepts anything)
  while(!Serial.available()){}
  input = Serial.readString();
  
  sample = magnetometer.getSample();

  angle = atan((sample.magnetic.y-avgY)/(sample.magnetic.x-avgX))*(180/M_PI);
  if((sample.magnetic.x-avgX)<0){
      angle = 270 - angle;
  } else if ((sample.magnetic.x-avgX)>0){
      angle = 90 - angle;
  } else if (sample.magnetic.y-avgY < 0){
      angle = 180.0;
  } else {
      angle = 0.0;
  }
  
  magnitude = sample.strength;

  // Prints angle and magnitude of vector at current location.
  Serial.println();
  Serial.print(String("Vector Data - "));
  Serial.print(String("Angle: " + String(angle) + "º , ")); 
  Serial.println(String("Magnitude: " + String(magnitude) + " uT"));
  Serial.println();
};