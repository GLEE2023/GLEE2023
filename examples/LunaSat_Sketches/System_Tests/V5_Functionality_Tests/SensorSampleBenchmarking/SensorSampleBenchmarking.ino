#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {1,1,1,1,1,1}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
LunaSat lunaSat(0, lunaSatConfig, false);

// Sample
lunaSat_sample_t sample;  

// Time variables for calculating sample time
float startTime; 
float endTime;

// False for performing tests using a single configuration, true for performing tests using all possible configurations
bool performAllTests = true;

/**
 * Parameters: None
 * Returns: None
 * This function handles calculating the average time to retrieve a sample for a given LunaSat configuration.
**/
void performTest(){
    Serial.println();
    Serial.print("LunaSat Configuration: ");    // Print current LunaSat Configuration
    Serial.print("{");
    Serial.print(String(lunaSatConfig[0])+",");
    Serial.print(String(lunaSatConfig[1])+",");
    Serial.print(String(lunaSatConfig[2])+",");
    Serial.print(String(lunaSatConfig[3])+",");
    Serial.print(String(lunaSatConfig[4])+",");
    Serial.print(String(lunaSatConfig[5])+"}");
    Serial.println();

    delay(100);
    
    //Retrieve a single sample
    Serial.println("Getting 1 Samples...");
    delay(100);

    startTime = micros();
    sample = lunaSat.getSample(); 
    endTime = micros();

    Serial.print("Time (in microseconds) to get 1 sample: "); Serial.println(endTime-startTime);
    
    //Retrieve 5 samples
    Serial.println("Getting 5 Samples...");
    delay(100);

    startTime = micros();
    for(int i = 0; i < 5; i++){
      sample = lunaSat.getSample();
    }
    endTime = micros();

    Serial.print("Time (in microseconds) to get 5 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in microseconds) to get 1 sample: "); Serial.println((endTime-startTime)/5);
     
    //Retrieve 10 samples
    Serial.println("Getting 10 Samples...");
    delay(100);

    startTime = micros();
    for(int i = 0; i < 10; i++){
      sample = lunaSat.getSample();
    }
    endTime = micros();

    Serial.print("Time (in microseconds) to get 10 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in microseconds) to get 1 sample: "); Serial.println((endTime-startTime)/10);

    //Retrieve 100 samples
    Serial.println("Getting 100 Samples...");
    delay(100);

    startTime = micros();
    for(int i = 0; i < 100; i++){
        sample = lunaSat.getSample();
    }
    endTime = micros();

    Serial.print("Average time (in microseconds) to get 1 sample: "); Serial.println((endTime-startTime)/100);
    delay(1000);
}

void setup() {
    Serial.begin(9600); // Direct serial communications with computer

    lunaSat.begin(9600);    
    delay(5000);
    
    if(!performAllTests){   // For a single configuration
        performTest();
    } else {                // For all configurations
        //https://www.geeksforgeeks.org/program-decimal-binary-conversion/
        lunaSatConfig[0] = lunaSatConfig[1] = lunaSatConfig[2] = lunaSatConfig[3] = lunaSatConfig[4] = lunaSatConfig[5] = 0;
        for(int n = 0; n < 64; n++){
            int j = 0;
            int k = n;
            while (k > 0) {
                lunaSatConfig[j] = k % 2;
                k = k / 2;
                j++;
            }
            for(int i = 0; i < 6; i++){
                lunaSat.info.conf[i] = lunaSatConfig[i];
            }
            performTest();

        }
        
    }
}

void loop() {
        
}