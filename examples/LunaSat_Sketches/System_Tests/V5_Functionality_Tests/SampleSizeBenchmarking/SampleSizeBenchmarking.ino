#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {1,1,1,1,1,1}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

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
    size = 0;
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
    Serial.println("Getting 1 Sample...");
    delay(100);

    sample = lunaSat.getSample(); 

    if(lunaSatConfig[0]==1){
      size = size + sizeof(sample.TMPtemperature);
    }
    if(lunaSatConfig[1]==1){
      size = size + sizeof(sample.acceleration);
    }
    if(lunaSatConfig[2]==1){
      size = size + sizeof(sample.magnetic);
    }
    if(lunaSatConfig[3]==1){
      size = size + sizeof(sample.TPTemperature);
    }
    if(lunaSatConfig[4]==1){
      size = size + sizeof(sample.cap);
    }
    
    Serial.print("Size of sample: "); Serial.print(size); Serial.println(" bytes");
    Serial.print("Timestamp size: "); Serial.print(sizeof(sample.timeStamp)); Serial.println(" bytes");
    Serial.print("Maximum size of sample: "); Serial.print(sizeof(sample)); Serial.println(" bytes");
    delay(1000);
}

void setup() {
  
    Serial.begin(9600);    // Direct serial communications with computer
    lunaSat.begin(9600);
    delay(5000);
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

void loop() {
        
}