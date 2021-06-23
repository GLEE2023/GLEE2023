#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {0,0,0,0,0,0}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
LunaSat lunaSat(0, lunaSatConfig, false);

// Sample
lunaSat_sample_t sample;  

// Time variables for calculating sample time
int size;

/**
 * Parameters: None
 * Returns: None
 * This function handles calculating the average time to retrieve a sample for a given LunaSat configuration.
**/
void performTest(){
    Serial.println();
    Serial.print("LunaSat Configuration: ");    // Print current LunaSat Configuration
    Serial.print("{");
    Serial.print(lunaSatConfig[0]); Serial.print(",");
    Serial.print(lunaSatConfig[1]); Serial.print(",");
    Serial.print(lunaSatConfig[2]); Serial.print(",");
    Serial.print(lunaSatConfig[3]); Serial.print(",");
    Serial.print(lunaSatConfig[4]); Serial.print(",");
    Serial.print(lunaSatConfig[5]); Serial.println("}");
    Serial.println();

    delay(100);

    //Retrieve a single sample
    Serial.println("Getting 1 Sample...");
    delay(100);

    sample = lunaSat.getSample(); 
    size = sizof(sample);

    Serial.print("Size of sample: "); Serial.print(size); Serial.println(" bytes")
}

void setup() {
    lunaSat.begin(9600);    // Direct serial communications with computer
    delay(5000);
    performTest();
}

void loop() {
        
}
