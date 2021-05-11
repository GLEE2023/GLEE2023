#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {1,0,0,0,0,0}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
LunaSat lunaSat(0, lunaSatConfig, false);

// Custom datatypes allow for sample specialization, user can craft their own ideal sample
lunaSat_sample_t sample;  

float startTime;
float endTime;

void setup() {
    // The GLEE library architecture enables easy, interpretable and familiar programming of the lunasat
    lunaSat.begin(9600);    // Direct serial communications with computer
    delay(5000);
}

void loop() {
    Serial.print("LunaSat Configuration: ");
    Serial.print("{");
    Serial.print(lunaSatConfig[0]);
    Serial.print(lunaSatConfig[1]);
    Serial.print(lunaSatConfig[2]);
    Serial.print(lunaSatConfig[3]);
    Serial.print(lunaSatConfig[4]);
    Serial.print(lunaSatConfig[5]); Serial.println("}");
    Serial.println("Getting 5 Samples...");
    delay(1000);
    // Simple fetching of sensor observation using lunasat class 
    // Later versions will allow for dynamic sampling based on user defined config
    startTime = micros();
    sample = lunaSat.getSample(); 
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    sample = lunaSat.getSample();
    endTime = micros();
    Serial.print("Time (in us) to get 5 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in us) to get 1 sample: "); Serial.println((endTime-startTime)/5);
    Serial.println("Getting 10 Samples...");
    delay(1000);
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
    Serial.print("Time (in us) to get 10 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in us) to get 1 sample: "); Serial.println((endTime-startTime)/10);
    Serial.println("Getting 100 Samples...");
    delay(1000);
    startTime = micros();
    for(int i = 0; i < 100; i++){
        sample = lunaSat.getSample();
    }
    endTime = micros();
    Serial.print("Time (in us) to get 100 samples: "); Serial.println(endTime-startTime);
    Serial.print("Average time (in us) to get 1 sample: "); Serial.println((endTime-startTime)/100);
    
    
}
