
#include <Arduino.h>

// Initialize capacitacive sensor address (Anolog pin connection #)
int cap_sensor_addr; 

// Raw ADC value returned when reading the sensor (10 Bit ADC -> 0-1023 values)
int raw_cap_ADC;

// Variable to store the 
int cap_voltage;

// Refrence Voltage
float ref_voltage;

void setup() {

    // Configuration

    // Sensor connected to A0
    cap_sensor_addr = A0;   

    // Refernce voltage of 3.3    
    ref_voltage = 3.3;      

    // TODO: Double check proper reference voltage configuration
    // analogReference(EXTERNAL); // This should set the reference voltage to be the 3.3V power supplied to the arduino

    // Begin serial communications with a baud rate of 9600 bps
    Serial.begin(9600); 
}

void loop() {

    // Read the analog ADC value of the capacitance sensor
    raw_cap_ADC = analogRead(cap_sensor_addr);

    // Calculate voltage based on 10 bit ADC (1023 possible values) and refrence voltage
    cap_voltage = raw_cap_ADC * (ref_voltage/1023.0); 

    // Print the calculate voltage via serial
    Serial.print("Capacitance Sensor Voltage: ");
    Serial.print(cap_voltage);
    Serial.println("V");

    // Option for printing sparse value on each line for ease of data processing/visualization with Serial Plotter interface
    // Serial.println(cap_voltage)

    delay(100);
}
