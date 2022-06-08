#include "MLX90393.h"

MLX90393 magnetometer = MLX90393(1,false); // Set-up for the magnetometer with debugging set to false

mlx_sample_t sample; // Saves the sample

float angle; // Saves the measured angle
float magnitude; // Saves the measured magnitude

float avgX = 0.0; // Saves average axes measurements
float avgY = 0.0;
float avgZ = 0.0;

String input = ""; // Variable reserved for user input

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
    Serial.println("Your LunaSat should be oriented such that the solar panels are at the top.");
    Serial.println("Press Enter to calibrate your magnetometer.");

    while(!Serial.available()){} // While no input from user, wait
    input = Serial.readString();

    // Get calibration points
    mlx_sample_t calibrationPoints[20];    
    for(int i = 0; i < 20; i++){
      calibrationPoints[i] = magnetometer.getSample();
    }

    float avgX = 0.0;
    float avgY = 0.0;
    float avgZ = 0.0;

    // Get sums of axes measurements
    for(int j = 0; j < 20; j++){
      avgX = avgX + calibrationPoints[j].magnetic.x;
      avgY = avgY + calibrationPoints[j].magnetic.y;
      avgZ = avgZ + calibrationPoints[j].magnetic.z;
    }

    // Find average axes measurements
    avgX = avgX/20;
    avgY = avgY/20;
    avgZ = avgZ/20;

    Serial.println("Calibration is complete. You may now begin the activity.");
    Serial.println();
}

void loop (){
  Serial.println("Place your LunaSat above the point where you wish to draw a vector.");
  Serial.println("Press Enter to generate a vector.");
  
  // Wait for continue command from user (currently accepts any input)
  while(!Serial.available()){}
  input = Serial.readString();

  // Gets sample from magnetometer
  sample = magnetometer.getSample();

  // Calculates angle of magnetic field vector
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
  magnitude = sqrt(pow(sample.magnetic.x-avgX,2) + pow(sample.magnetic.y-avgY,2));

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
