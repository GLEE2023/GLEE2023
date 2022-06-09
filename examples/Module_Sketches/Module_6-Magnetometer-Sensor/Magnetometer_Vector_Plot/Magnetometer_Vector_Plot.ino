#include "MLX90395.h"

const int NUM_SAMPLES = 20;

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
    //magnetometer.setGain(gainMultipliers[0]);

    // Set resolution
    magnetometer.setResolution(MLX90395_RES_19,MLX90395_RES_19,MLX90395_RES_16);

    // Set oversampling
    magnetometer.setOSR(MLX90395_OSR_2);

    // Set digital filtering
    magnetometer.setFilter(MLX90395_FILTER_6);

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
  // if((valX-calX)<0){
  //     angle = 270 - angle;
  // } else if ((valX-calX)>0){
  //     angle = 90 - angle;
  // } else if (valY-calY < 0){
  //     angle = 180.0;
  // } else {
  //     angle = 0.0;
  // }
  String direction_str = "";

  // Determine the direction of magnetic field vector based on the angle

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
  Serial.print(String("Direction: " + direction_str + " , "));

  // Length calculations
  // A vector of magnitude 1000.0 uT translates to a 2cm vector,
  // but this is not a strictly defined limit
  magnitude = ((2) / (1000.0)) * (magnitude);
  Serial.println(String("Magnitude: " + String(magnitude) + " cm"));
  Serial.println(String("(Average values: X = " + String(valX-calX) + " Y = " + String(valY-calY) + " Angle = " + String(angle) + ")"));
  Serial.println();
}
