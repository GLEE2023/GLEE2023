#include "GLEE_Radio.h"
#include "Zanshin_BME680.h" 

LunaRadio Rad;
BME680_Class BME680;
float altitude(const int32_t press, const float seaLevel = 1013.25);
float altitude(const int32_t press, const float seaLevel) {
  /*!
  @brief     This converts a pressure measurement into a height in meters
  @details   The corrected sea-level pressure can be passed into the function if it is known,
             otherwise the standard atmospheric pressure of 1013.25hPa is used (see
             https://en.wikipedia.org/wiki/Atmospheric_pressure) for details.
  @param[in] press    Pressure reading from BME680
  @param[in] seaLevel Sea-Level pressure in millibars
  @return    floating point altitude in meters.
  */
  static float Altitude;
  Altitude =
      44330.0 * (1.0 - pow(((float)press / 100.0) / seaLevel, 0.1903));  // Convert into meters
  return (Altitude);
}  // of method altitude()


void setup() {
  Serial.begin(115200);
#ifdef __AVR_ATmega32U4__      // If this is a 32U4 processor, then wait 3 seconds to init USB port
  delay(3000);
#endif
  Serial.print(F("- Initializing BME680 sensor\n"));
  while (!BME680.begin(I2C_STANDARD_MODE))  { // Start BME680 using I2C, use first device found
    Serial.print(F("-  Unable to find BME680. Trying again in 5 seconds.\n"));
    delay(5000);
  }  // of loop until device is located
  Serial.print(F("- Setting 16x oversampling for all sensors\n"));
  BME680.setOversampling(TemperatureSensor, Oversample16);  // Use enumerated type values
  BME680.setOversampling(HumiditySensor, Oversample16);     // Use enumerated type values
  BME680.setOversampling(PressureSensor, Oversample16);     // Use enumerated type values
  Serial.print(F("- Setting IIR filter to a value of 4 samples\n"));
  BME680.setIIRFilter(IIR4);  // Use enumerated type values
  Serial.print(F("- Setting gas measurement to 320\xC2\xB0\x43 for 150ms\n"));  // "�C" symbols
  BME680.setGas(320, 150);  // 320�c for 150 milliseconds

  Rad.initialize_radio(915.0,17,250.0,12,8);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static int32_t  temp, humidity, pressure, gas;  // BME readings
  static float    alt;                            // Temporary variable
  static char buff[256];
  BME680.getSensorData(temp, humidity, pressure, gas);   // Get readings
  alt = altitude(pressure);
  snprintf(buff, sizeof(buff), "Temperature: %3d.%02d \xC2\xB0\x43 Humidity: %3d.%03d%% Pressure: %5d.%02d hPa Altitude: %5d.%02d m Air: %4d.%02d m\xE2\x84\xA6",
     (int8_t)(temp / 100), (uint8_t)(temp % 100),
     (int8_t)(humidity / 1000),(uint16_t)(humidity % 1000),
     (int16_t)(pressure / 100),(uint8_t)(pressure % 100),
     (int16_t)(alt), ((uint8_t)(alt * 100) % 100),
     (int16_t)(gas / 100), (uint8_t)(gas % 100));
  Serial.println(buff);
  Rad.transmit_data(buff);
  delay(1000);  // Wait 10s
}
