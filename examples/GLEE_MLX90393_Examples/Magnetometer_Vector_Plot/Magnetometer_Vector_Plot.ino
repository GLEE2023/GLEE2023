#include "MLX90393.h"

MLX90393 magnetometer = MLX90393(1,false);

mlx_sample_t sample;

const int numOfPoints = 4;

String x_coordinates[numOfPoints];
String y_coordinates[numOfPoints];
mlx_sample_t data[numOfPoints];

float tmp = 0.0;


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

    String tmp;
    String mTp;
    for (int i = 0; i < numOfPoints; i++){
        Serial.println("Move LunaSat and then enter x coordinate of LunaSat relative to magnet. Do not rotate LunaSat while moving it.");
        while(!(Serial.available() > 0)){} // Rather than collecting data automatically, input can be used to gather data at chosen time by user
        tmp = Serial.readString();
        mTp = tmp;
        mTp.trim();
        x_coordinates[i] = mTp;
        Serial.println("Enter y coordinate of LunaSat relative to magnet. Do not move LunaSat.");
        while(!(Serial.available() > 0)){} // Rather than collecting data automatically, input can be used to gather data at chosen time by user
        tmp = Serial.readString();
        mTp = tmp;
        mTp.trim();
        y_coordinates[i] = mTp;
        data[i] = magnetometer.getSample();
       
        delay(1000); // Take samples every one second
    }
    // Print out coordinates
    Serial.println("Coordinates");
    for(int i = 0; i < numOfPoints; i++){
      Serial.print("(");Serial.print(x_coordinates[i]);Serial.print(",");Serial.print(y_coordinates[i]);Serial.println(")");
    }
    Serial.println();
    // Print out relevant measurements and calcualtions
    Serial.println("Axes Measurements (xAxis,yAxis,zAxis,angle,fieldStrength)");
    for(int i = 0; i < numOfPoints; i++){
      Serial.print(data[i].magnetic.x); Serial.print("uT, "); 
      Serial.print(data[i].magnetic.y); Serial.print("uT, "); 
      Serial.print(data[i].magnetic.z); Serial.print("uT, ");
      float angle = atan(data[i].magnetic.y/data[i].magnetic.x)*(180/M_PI);
      if(data[i].magnetic.y<0){
        angle = 270 - angle;
      } else {
        angle = 90 - angle;
      }
      Serial.print(angle); Serial.print("º, ");
      Serial.print(data[i].strength); Serial.println("uT");
    }
    Serial.println();
    // Print out vectors to be drawn by hand
    Serial.println("Vectors to draw");
    for(int i = 0; i < numOfPoints; i++){
      Serial.print("Vector: <");Serial.print(data[i].magnetic.x);
      Serial.print(","); Serial.print(data[i].magnetic.y);
      Serial.print(">, Location: ("); Serial.print(x_coordinates[i]);
      Serial.print(",");Serial.print(y_coordinates[i]);Serial.print(")");
      float angle = atan(data[i].magnetic.y/data[i].magnetic.x)*(180/M_PI);
      if(data[i].magnetic.y<0){
        angle = 270 - angle;
      } else {
        angle = 90 - angle;
      }
      Serial.print(", Angle: "); Serial.print(angle); Serial.print("º");
      Serial.print(", Magnitude: ");Serial.print(data[i].strength);Serial.println(" uT");
    }

};

void loop (){   
    
};