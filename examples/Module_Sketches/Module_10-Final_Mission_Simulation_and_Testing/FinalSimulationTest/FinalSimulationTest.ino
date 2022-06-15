// Sensor Includes
#include "TMP117.h"
#include "MLX90395.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"

// Sensor Objects (Debugging defaults to false)
TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90395 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

// Sensor Data Variables
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;

// Output Formatting Variables
String printOut;
String sep = ", ";
unsigned long timeStamp;

int LED1 = 4;
int LED2 = 5;


void setup() {
    // Start up Serial (wired) Communications
    Serial.begin(9600);

    // Sensor Initializations

    // Temperature Sensor - TMP117 (Nothing)

    // Accelerometer - MPU6000
    accelerometer.begin(); // Begin (check connection)
    accelerometer.initialize(); // Initialize (setup)
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Make sure acceleration range is set to 2G

    // Magnetometer - MLX90393
    magnetometer.begin_I2C();
    // Set gain, resolution,OSR, and digital filter
    magnetometer.setGain(8); //default value is 8, can range from 0-15
    magnetometer.setResolution(MLX90395_RES_17,MLX90395_RES_17,MLX90395_RES_17);
    magnetometer.setOSR(MLX90395_OSR_4); //not default. default is MLX90395_OSR_1 and is equivalent to 0
    magnetometer.setFilter(MLX90395_FILTER_8); //default. MLX90395_FILTER_1 is equivalent to 0

    // Capacitive Sensor CAP
    capacitive.begin(); // Begin

    // Thermopile - TPIS1385
    thermopile.begin(); // Begin
    thermopile.readEEprom(); // Check Registers

    // Print out data header
    Serial.println("Sample Key: ");
    Serial.println("Time, TMP117, MPU6000-X, MPU6000-Y, MPU6000-Z, MLX90393-X, MLX90393-Y, MLX90393-Z, TPIS1385-Object, TPIS1385-Ambient, CAP");

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);


}

void loop() {
    // Save Timestamp
    timeStamp = millis();

    // Get samples from each sensor
    tempSample = thermometer.getTemperatureC();
    accSample = accelerometer.getSample();
    magSample = magnetometer.getSample();
    thermSample = thermopile.getSample();
    capSample = capacitive.getRawData();

    // Translate sample data to Strings, combine with seperators (", ") inbetween
    printOut = String(timeStamp) + sep + String(tempSample) + sep + String(accSample.x) + sep +
                String(accSample.y) + sep + String(accSample.z) + sep + String(magSample.magnetic.x) + sep +
                String(magSample.magnetic.y) + sep + String(magSample.magnetic.z) + sep +
                String(thermSample.object,5) + sep + String(thermSample.ambient,5) + sep + String(capSample);

    // Print the
    Serial.println(printOut);

    //LED blink test
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);
    delay(1000); // change delays to change sample rate
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    delay(1000);// change delays to change sample rate

    // Repeate every ~500ms INCREASE TO CHANGE SAMPLE RATE
    //delay(500);
}
