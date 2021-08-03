#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {1,1,1,1,1,1}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
LunaSat lunaSat(0, lunaSatConfig, true);

// Custom datatypes allow for sample specialization, user can craft their own ideal sample
lunaSat_sample_t sample;  

void setup() {
    // The GLEE library architecture enables easy, interpretable and familiar programming of the lunasat
    lunaSat.begin(9600);    // Direct serial communications with computer
    delay(5000);
}

void loop() {
    // Simple fetching of sensor observation using lunasat class 
    // Later versions will allow for dynamic sampling based on user defined config
    sample = lunaSat.getSample(); 

    // Simple examples of interacting with an observation sample
    lunaSat.dispSample(sample); // Observation samples can be directly displayed via serial
    
    delay(100);
}
