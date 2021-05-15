#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {0,0,0,0,0,0}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
LunaSat lunaSat(0, lunaSatConfig, false);

// Sample
lunaSat_sample_t sample;  

// Time variables for calculating sample time
float startTime; 
float endTime;

// False for performing tests using a single configuration, true for performing tests using all possible configurations
bool performAllTests = false;


/**
 * Parameters: None
 * Returns: None
 * This function handles calculating the average time to retrieve a sample for a given LunaSat configuration.
**/
void performTest(){
    Serial.print("LunaSat Configuration: ");    // Print current LunaSat Configuration
    Serial.print("{");
    Serial.print(lunaSatConfig[0]); Serial.print(",");
    Serial.print(lunaSatConfig[1]); Serial.print(",");
    Serial.print(lunaSatConfig[2]); Serial.print(",");
    Serial.print(lunaSatConfig[3]); Serial.print(",");
    Serial.print(lunaSatConfig[4]); Serial.print(",");
    Serial.print(lunaSatConfig[5]); Serial.println("}");
    Serial.println();

    //Serial.println("LunaSat Configuration: {" + to_string(lunaSatConfig[0]) + "," + to_string(lunaSatConfig[1]) + "," + to_string(lunaSatConfig[2]) + "," + to_string(lunaSatConfig[3]) + "," + to_string(lunaSatConfig[4]) + "," + to_string(lunaSatConfig[5]) + "}");

    delay(100);

    Serial.println("Getting 1 Samples...");
    delay(100);

    startTime = micros();
    sample = lunaSat.getSample(); 
    endTime = micros();

    Serial.print("Time (in microseconds) to get 1 sample: "); Serial.println(endTime-startTime);

    Serial.println("Getting 5 Samples...");
    delay(100);

    startTime = micros();
    sample = lunaSat.getSample(); 
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    endTime = micros();

    Serial.print("Time (in microseconds) to get 5 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in microseconds) to get 1 sample: "); Serial.println((endTime-startTime)/5);

    Serial.println("Getting 10 Samples...");
    delay(100);

    startTime = micros();
    sample = lunaSat.getSample(); 
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample(); 
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    endTime = micros();

    Serial.print("Time (in microseconds) to get 10 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in microseconds) to get 1 sample: "); Serial.println((endTime-startTime)/10);

    Serial.println("Getting 100 Samples...");
    delay(100);

    startTime = micros();
    for(int i = 0; i < 100; i++){
        sample = lunaSat.getSample();
    }
    endTime = micros();

    Serial.print("Time (in microseconds) to get 100 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in microseconds) to get 1 sample: "); Serial.println((endTime-startTime)/100);
}

void setup() {
    lunaSat.begin(9600);    // Direct serial communications with computer
    delay(5000);

    if(!performAllTests){   // For a single configuration
        performTest();
    } else {                // For all configurations
        Serial.println("Performing all possible tests.");
        Serial.println();

        for(int i = 0; i<6; i++){   // Set LunaSat configuration to all sensors disabled
            lunaSatConfig[i] = 0;
        }

        //Perform benchmarking tests on all 64 combinations of sensors
        for(int zero = 0; zero < 2; zero++){
            for(int one = 0; one < 2; one++){
                for(int two = 0; two < 2; two++){
                    for(int three = 0; three < 2; three++){
                        for(int four = 0; four < 2; four++){
                            for(int five = 0; five < 2; five++){
                                lunaSatConfig[5] = five;
                                performTest();
                            }
                            lunaSatConfig[4] = four;
                            performTest();
                        }
                        lunaSatConfig[3] = three;
                        performTest();
                    }
                    lunaSatConfig[2] = two;
                    performTest();
                }
                lunaSatConfig[1] = one;
                performTest();
            }
            lunaSatConfig[0] = zero;
            performTest();
        }
    }
}

void loop() {
        
}
