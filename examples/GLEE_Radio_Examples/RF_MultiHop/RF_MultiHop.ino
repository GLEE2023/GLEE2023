#include <GLEE_Radio.h>

// Initialize RF Object
LunaRadio Rad;

int sizeOfBuffer = 20; //comfortable max is around 1500
char buff[20] = {'\0'};
char tempBuffer[5] = {' '};

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
}

void loop() {
  //Check if RF successfully received tranmsission using the recieve_data_string() function
  //Store Results in a string variable
  numTransmissions++;
  Serial.println(numTransmissions);
  String received = Rad.receive_data_string();
  Serial.println("Received: " + received);

  //Add to buffer
  strcat(buff, received); //appends recevied message to buff
  Serial.println("Buff: " + buff);
  strcat(buff,","); // appends comma to buff
  //Transmit to another LunaSat
  //Rad.transmit_data();
  if(buff[sizeOfBuffer - 4] != NULL){
    Serial.println("Maximum memory reached. Stopping receipt and transmission.");
    delay(500);
    exit(0);
  }
}
