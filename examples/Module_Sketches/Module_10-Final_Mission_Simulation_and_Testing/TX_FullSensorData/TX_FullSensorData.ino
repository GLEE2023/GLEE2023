// Sensor Includes
#include "TMP117.h"
#include "MLX90393.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"
#include "GLEE_Radio.h"

// Initiate LunaSat Radio (SX1272 Transciever)
LunaRadio Rad;

// Sensor Objects (Debugging defaults to false)
TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90393 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

// Sensor Data Variables
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;

// Individual string buffers for memory optimization
char timeStamp_str[8] = {' '};
char temp_str[8] = {' '} ;
char accX_str[8] = {' '} ;
char accY_str[8] = {' '} ;
char accZ_str[8] = {' '} ;
char magX_str[7] = {' '} ;
char magY_str[7] = {' '} ;
char magZ_str[7] = {' '} ;
char thermObj_str[6] = {' '} ;
char thermAmb_str[6] = {' '} ;
char cap_str[5] = {' '};

// Transmission Buffer
char data_buffer[80]; 

// Output Formatting Variables
String printOut;
char sep = ",";
unsigned long timeStamp;

// RED Led Pin
int LED = 4;

void setup() {
    // Start up Serial (wired) Communications
    Serial.begin(9600);

    // SENSOR INITIALIZATIONS

    // Temperature Sensor - TMP117 
    // (Nothing)
    
    // Accelerometer - MPU6000
    accelerometer.begin(); // Begin (check connection)
    accelerometer.initialize(); // Initialize (setup)
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Make sure acceleration range is set to 2G

    // Magnetometer - MLX90393    
    magnetometer.begin_I2C(); // Begin
    magnetometer.setGain(MLX90393_GAIN_2_5X); // Set gain (signal amplification)
    magnetometer.setResolution(MLX90393_X, MLX90393_RES_19); // Set resolutions on all 3 axis
    magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16);
    magnetometer.setOversampling(MLX90393_OSR_2); // Set oversampling
    magnetometer.setFilter(MLX90393_FILTER_6); // Set digital filtering

    // Capacitive Sensor CAP
    capacitive.begin(); // Begin

    // Thermopile - TPIS1385
    thermopile.begin(); // Begin
    thermopile.readEEprom(); // Check Registers

    // Initilaize Radio, turn up power (max 17) to improve range
    Rad.initialize_radio(915.0, 7, 250.0, 12, 8);

    // Set RED led pin to output
    pinMode(LED, OUTPUT);
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

    // Decimal TO STRing Functions to load decimal (float) samples as strings into individual buffers
    // see https://www.programmingelectronics.com/dtostrf/ for more info
    dtostrf(tempSample,3,3,temp_str); 

    dtostrf(accSample.x,1,4,accX_str);
    dtostrf(accSample.y,1,4,accY_str); 
    dtostrf(accSample.z,1,4,accZ_str); 

    dtostrf(magSample.magnetic.x,2,2,magX_str);
    dtostrf(magSample.magnetic.y,2,2,magY_str);
    dtostrf(magSample.magnetic.z,2,2,magZ_str);

    dtostrf(thermSample.object,3,2,thermObj_str);
    dtostrf(thermSample.ambient,3,2,thermAmb_str);

    // Using sprintf to load integers and unsigned longs (time) into buffers as strings
    sprintf(timeStamp_str, "%lu", timeStamp);
    sprintf(cap_str, "%i", capSample);

    // Using STRing CoPY to construction of transmission buffer with ID for transmission filtering
    
    ///////////////////////////////////////////////
    strcpy(data_buffer, "11"); // CHANGE TO YOUR TEAM NUMBER
    ///////////////////////////////////////////////

    // Using STRing ConCaT to append indiviual character arrays (psudo strings) to 
    strcat(data_buffer, timeStamp_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, temp_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, accX_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, accY_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, accZ_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, magX_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, magY_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, magZ_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, thermObj_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, thermAmb_str); 
    strcat(data_buffer, ",");
    strcat(data_buffer, cap_str); 

    // Print data (already in string format) to serial monitor(wired)
    Serial.println(data_buffer);

    // Transmit string data (already in string format) to 
    Rad.transmit_data(data_buffer);

    // Note: Optimized transmission of data is in progress... String formatting for demo purposes

    // Blink after transmit (Remove for faster datarate)
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
  
     // Repeate every ~500ms INCREASE TO CHANGE SAMPLE RATE
    delay(500);
}