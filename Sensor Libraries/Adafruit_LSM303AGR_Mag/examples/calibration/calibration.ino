#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303AGR_Mag.h>


Adafruit_LSM303AGR_Mag_Unified mag = Adafruit_LSM303AGR_Mag_Unified(12345);

float MagMinX, MagMaxX;
float MagMinY, MagMaxY;
float MagMinZ, MagMaxZ;

long lastDisplayTime;

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("LSM303 Calibration"); Serial.println("");
  
  /* Initialise the magnetometer */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  lastDisplayTime = millis();
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t magEvent; 
  
  mag.getEvent(&magEvent);
  
  if (magEvent.magnetic.x < MagMinX) MagMinX = magEvent.magnetic.x;
  if (magEvent.magnetic.x > MagMaxX) MagMaxX = magEvent.magnetic.x;
  
  if (magEvent.magnetic.y < MagMinY) MagMinY = magEvent.magnetic.y;
  if (magEvent.magnetic.y > MagMaxY) MagMaxY = magEvent.magnetic.y;

  if (magEvent.magnetic.z < MagMinZ) MagMinZ = magEvent.magnetic.z;
  if (magEvent.magnetic.z > MagMaxZ) MagMaxZ = magEvent.magnetic.z;

  if ((millis() - lastDisplayTime) > 1000)  // display once/second
  {
    Serial.print("Mag Minimums: "); Serial.print(MagMinX); Serial.print("  ");Serial.print(MagMinY); Serial.print("  "); Serial.print(MagMinZ); Serial.println();
    Serial.print("Mag Maximums: "); Serial.print(MagMaxX); Serial.print("  ");Serial.print(MagMaxY); Serial.print("  "); Serial.print(MagMaxZ); Serial.println(); Serial.println();
    lastDisplayTime = millis();
  }
}
