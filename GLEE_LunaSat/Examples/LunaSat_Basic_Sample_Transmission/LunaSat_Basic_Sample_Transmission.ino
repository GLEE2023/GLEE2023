#include <Arduino.h>

// The GLEE team is developing library architecture to enable easy programming of the lunasat 
#include <LunaSat.h>    
#include <GLEE_Radio.h> // Library containing pertiant radio functionality

LunaRadio Rad;

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
// Can be easily expanded to account for different axes of LunaSat configuration
LunaSat lunaSat(0, true);

// Custom datatypes allow for sample specialization, user can craft their own ideal sample
lunaSat_sample_t sample;  

void setup() {
    // The GLEE library architecture enables easy, interpretable and familiar programming of the lunasat
    lunaSat.begin(9600);    // Direct serial communications with computer
    // Heres an example of applying configuration to the RX module via paramaterization 
    Rad.initialize_radio(915.0,17,250.0,12,8); 
}

void loop() {
    // Simple fetching of sensor observation using lunasat class 
    // Later versions will allow for dynamic sampling based on user defined config
    sample = lunaSat.getSample(); 

    // Simple examples of interacting with an observation sample
    lunaSat.dispSample(sample); // Observation samples can be directly displayed via serial
    Rad.transmit_data(sample);  // Observation samples can be just as easily transmitted to a reciever
    delay(2000);
}
