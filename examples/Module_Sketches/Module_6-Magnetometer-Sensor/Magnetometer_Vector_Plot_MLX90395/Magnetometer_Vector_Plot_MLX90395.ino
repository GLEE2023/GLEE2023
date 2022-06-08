#include "MLX90395.h"

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

    // Calibration
    Serial.println("Place your LunaSat at your desk with the magnet away.");
    Serial.println("Your LunaSat should be oriented such that the solar panels are at the top.");
    Serial.println("Press Enter to calibrate your magnetometer.");
    while(!Serial.available()){} // While no input from user, wait
    input = Serial.readString();

    // Get calibration points
    mlx_sample_t calibrationPoints[20];
    for(int i = 0; i < 20; i++){
      calibrationPoints[i] = magnetometer.getSample();
      delay(100); //Wait to ensure mag has new measurement
    }

    float calX,sumX = 0.0;
    float calY,sumY = 0.0;
    float calZ,sumZ = 0.0;

    // Get sums of axes measurements
    for(int j = 0; j < 20; j++){
      sumX = sumX + calibrationPoints[j].magnetic.x;
      sumY = sumY + calibrationPoints[j].magnetic.y;
      sumZ = sumZ + calibrationPoints[j].magnetic.z;
    }

    // Find average axes measurements to use as calibration
    calX = sumX/20;
    calY = sumY/20;
    calZ = sumZ/20;

    Serial.println("Calibration is complete. You may now begin the activity.");
    Serial.println();
}


void loop (){
  Serial.println("Place your LunaSat above the point where you wish to draw a vector.");
  Serial.println("Press Enter to generate a vector.");

  // Wait for continue command from user (currently accepts any input)
  while(!Serial.available()){}
  input = Serial.readString();

  // Get samples from magnetometer
  mlx_sample_t samples[20];
  for(int i = 0; i < 20; i++){
    samples[i] = magnetometer.getSample();
    delay(100); //Wait to ensure mag has new measurement
  }

  float sumX = 0.0;
  float sumY = 0.0;
  float sumZ = 0.0;

  // Get sums of axes measurements
  for(int j = 0; j < 20; j++){
    sumX = sumX + samples[j].magnetic.x;
    sumY = sumY + samples[j].magnetic.y;
    sumZ = sumZ + samples[j].magnetic.z;
  }

  float valX = sumX/20;
  float valY = sumY/20;
  float valZ = sumZ/20;

  // Calculates angle of magnetic field vector
  angle = atan((valY-calY)/(valX-calX))*(180/M_PI);
  if((valX-calX)<0){
      angle = 270 - angle;
  } else if ((valX-calX)>0){
      angle = 90 - angle;
  } else if (valY-calY < 0){
      angle = 180.0;
  } else {
      angle = 0.0;
  }
  String direction_str = "";

  // Determine the direction of magnetic field vector based on the angle
  if (angle <= 22.5 || angle > 337.5){
      direction_str = "Up";
  } else if (angle <= 67.5 && angle > 22.5){
      direction_str = "Up Left";
  } else if (angle <= 112.5 && angle > 67.5){
      direction_str = "Left";
  } else if (angle <= 157.5 && angle > 112.5){
      direction_str = "Down Left";
  } else if (angle <= 202.5 && angle > 157.5){
      direction_str = "Down";
  } else if (angle <= 247.5 && angle > 202.5){
      direction_str = "Down Right";
  } else if (angle <= 292.5 && angle > 247.5){
      direction_str = "Right";
  } else if (angle <= 337.5 && angle > 292.5){
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
  Serial.println();
}
