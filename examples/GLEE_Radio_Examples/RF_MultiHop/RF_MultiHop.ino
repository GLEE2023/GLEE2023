#include <GLEE_Radio.h>

// Initialize RF Object
LunaRadio Rad;

int sizeOfBuffer = 40; //comfortable max is around 1500
char output[5] = {'\0'};
int lenBuff = 0;
String received = "";

// conversion for transmission
char MSG[16];

int sampleRate; // Sample rate in samples per second (Hz)
float startTime;    // Variables for timing offset calculations
float endTime;

int numTransmissions = 0;

void setup() {
  // Set the data rate to 9600 bits per second
  Serial.begin(9600);

    // Initialize Transciever with following parameters
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0, 17, 250.0, 12, 8);
    delay(50);

    sampleRate = 1;
}

void loop() {
  startTime = millis(); //Check if RF successfully received tranmsission using the recieve_data_string() function
  numTransmissions++;
  Serial.println(numTransmissions);
  received = Rad.receive_data_string();
  Serial.println("Received: " + received);
  delay(500);

  lenBuff += 4;
  // Check buffer size 
  if(sizeOfBuffer - lenBuff < 4){
    Serial.println("Maximum memory reached. Stopping receipt and transmission.");
    delay(500);
    exit(0);
  }
  
  endTime = millis();
  delay(sampleRate * 3000 - (endTime - startTime));

  //Transmit to another LunaSat
  received.toCharArray(MSG,8);
  Rad.transmit_data(MSG);
}
