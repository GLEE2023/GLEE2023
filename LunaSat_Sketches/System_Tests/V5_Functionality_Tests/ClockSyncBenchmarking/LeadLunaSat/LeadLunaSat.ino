#include <SoftwareSerial.h>
​
const byte rxPin = 2;
const byte txPin = 3;
​
// set up a new serial object
SoftwareSerial serialComms (rxPin, txPin);
​
// disable interrupt when it's not needed
volatile bool interruptEnabled = true;
​
String requestPacket;
​
// Assemble LunaSat Packet
// Response char array and string variables
char RSP[80];
String rsp;
​
unsigned long localTime;
String localTime_string;
​
unsigned long timeReceived;
String timeReceived_string;
​
void setup() {
    //Set the data rate to 9600 bits per second
    Serial.begin(9600);
    while(!Serial){} // Wait for serial to initialize
​
    // define pin modes for tx, rx:
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    // set the data rate for the SoftwareSerial port
    serialComms.begin(9600); // software serial = incoming data port from client
}
​
void loop(){
    localTime = micros();
    
    if(serialComms.available() > 0){
      requestPacket = serialComms.readString(); // Read incoming packet
​
      Serial.println(requestPacket); // forward incoming packet to computer for monitoring
​
    // only respond if it's a sync request
    if(requestPacket == "syncRequest"){
        // Disable interrupts during reception processing
        interruptEnabled = false;
    ​
        timeReceived = micros(); //Change to seconds or microseconds for different tests
        timeReceived_string = String(timeReceived);
        
        // Send back time received and time sent
        localTime = micros(); //Change to seconds or microseconds for different tests
        localTime_string = String(localTime);
    ​
        rsp = String(timeReceived_string + "," + localTime_string);
        rsp.toCharArray(RSP,80);
        serialComms.print(RSP); // send response packet to client over software serial
        
        Serial.println(RSP); // send to computer over hardware serial for monitoring
    ​
        interruptEnabled = true;
    }
      
    }
}