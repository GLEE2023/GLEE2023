#include "MLX90395.h"

//Take NUM_SAMPLES samples and average them to return a direction
const int NUM_SAMPLES = 5;

MLX90395 magnetometer = MLX90395(1,false); // Set-up for the magnetometer with debugging set to false

mlx_sample_t sample; // Saves the sample
float angle; // Saves the measured angle
float magnitude; // Saves the measured magnitude
float calX = 0.0; // Saves average axes measurements
float calY = 0.0;
float calZ = 0.0;
String input = ""; // Variable reserved for user input

void setup (){
    Serial.begin(9600);

    magnetometer.begin_I2C();

    // Set gain
    magnetometer.setGain(8); //default value is 8, can range from 0-15
    // Set resolution
    magnetometer.setResolution(MLX90395_RES_17,MLX90395_RES_17,MLX90395_RES_17);
    // Set oversampling. See 18.4 in the datasheet
    magnetometer.setOSR(MLX90395_OSR_4); //not default. default is MLX90395_OSR_1 and is equivalent to 0
    // Set digital filtering
    magnetometer.setFilter(7); //default. MLX90395_FILTER_6 is equivalent to 0

    // Remove bias in axes
    Serial.println("Press Enter to calibrate LunaSat.\nKeep LunaSat a few feet away from the magnet.");

    //waaaiit
    while(!Serial.available()){}
    input = Serial.readString();

    sample = magnetometer.getSample();
    calX = sample.magnetic.x;
    calY = sample.magnetic.y;
    calZ = sample.magnetic.z;

    delay(100);
    Serial.println("Begin Activity");
}


void loop (){
  Serial.println("Place your LunaSat above the point where you wish to draw a vector.");
  Serial.println("Press Enter to generate a vector.");

  // Wait for continue command from user (currently accepts any input)
  while(!Serial.available()){}
  input = Serial.readString();

  // Get samples from magnetometer
  mlx_sample_t samples[NUM_SAMPLES];
  for(int i = 0; i < NUM_SAMPLES; i++){
    samples[i] = magnetometer.getSample();
    Serial.print(".");
    delay(100); //Wait to ensure mag has new measurement
  }
  Serial.println();

  float sumX = 0.0;
  float sumY = 0.0;
  float sumZ = 0.0;

  // Get sums of axes measurements
  for(int j = 0; j < NUM_SAMPLES; j++){
    sumX = sumX + samples[j].magnetic.x;
    sumY = sumY + samples[j].magnetic.y;
    sumZ = sumZ + samples[j].magnetic.z;
  }

  float valX = sumX/NUM_SAMPLES;
  float valY = sumY/NUM_SAMPLES;
  float valZ = sumZ/NUM_SAMPLES;

  // Calculates angle of magnetic field vector
  angle = atan2((valY-calY),(valX-calX))*180/3.14;

  // Determine the direction of magnetic field vector based on the angle
  String direction_str = "";
  if (angle <= 22.5 && angle > -22.5){
      direction_str = "Right";
  } else if (angle <= 67.5 && angle > 22.5){
      direction_str = "Down Right";
  } else if (angle <= 112.5 && angle > 67.5){
      direction_str = "Down";
  } else if (angle <= 157.5 && angle > 112.5){
      direction_str = "Down Left";
  } else if (angle <= -157.5 || angle > 157.5){
      direction_str = "Left";
  } else if (angle <= -113.5 && angle > -157.5){
      direction_str = "Up Left";
  } else if (angle <= -67.5 && angle > -112.5){
      direction_str = "Up";
  } else if (angle <= -22.5 && angle > -67.5){
      direction_str = "Up Right";
  }

  // Calculates magnitude of vector from x and y axes measurements only
  magnitude = sqrt(pow(valX-calX,2) + pow(valY-calY,2));

  // Prints direction and magnitude of vector at current location.
  Serial.println();
  Serial.print(String("Vector Data - "));
  Serial.println(String("Direction: " + direction_str));
  Serial.println(String("(Average values: X = " + String(valX-calX) + " Y = " + String(valY-calY) + " Angle = " + String(angle) + ")"));
  Serial.println();
}
