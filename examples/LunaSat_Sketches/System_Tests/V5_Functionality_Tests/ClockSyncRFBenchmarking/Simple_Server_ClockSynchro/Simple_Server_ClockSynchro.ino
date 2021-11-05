#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

String id = "L1";

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

// Assemble LunaSat Packet
// Response char array and string variables
char RSP[32];
String rsp;

float localTime; // This is also unsigned
String localTime_string;

unsigned long timeReceived; // Check here first! This is unsigned here, but signed in the client code.
String timeReceived_string;

long timeSinceLastUpdate;

//For storing output pin configuration of LEDs
int LED1 = 4; 
int LED2 = 5; 

void recieve_callback(void) {
  // don't set flag if interrupt isn't enabled
  if(!interruptEnabled) {
    return;
  }

  // set flag signifying message was recieved
  messageRecieved = true;
}

void setup() {
    //Set the data rate to 9600 bits pere second
    Serial.begin(9600);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    //Initialize the radio settings by using the initialize_radio function
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0,17,250.0,12,8);

    Rad.enable_recieve_interupt(recieve_callback);
    Serial.println(interruptEnabled);
    localTime = millis();
}

void loop(){
    //timeSinceLastUpdate = millis() - localTime;
    localTime += (millis()-localTime); // Change to microseconds for different tests
    if((localTime % 1000 > 310) && (localTime % 1000 < 315)){ // Change to microseconds for different tests
        //Blink LED
        digitalWrite(LED2, HIGH);
    } else if ((localTime % 1000 > 510) && (localTime % 1000 < 515)){
        digitalWrite(LED2, LOW);
    }
    if(messageRecieved){
        localTime += (millis()-localTime);
        // Disable interrupts during reception processing
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;
        //timeSinceLastUpdate = millis() - localTime;
        
        timeReceived = localTime; // Change to microseconds for different tests
        

        //Read data from request
        char response[32];
        Rad.readData(response, 32);
        
        String head = String(response[0])+String(response[1]);

        timeReceived_string = String(timeReceived);
        //If the request matches the time request flag, send back time received and time sent
        if(head=="L2"){
            rsp = "";
            Serial.println(F("Recieved request from LunaSat, sending response"));
            //timeSinceLastUpdate = millis() - localTime;
            localTime += (millis()-localTime); // Change to microseconds for different tests
            localTime_string = String(localTime); // Time sent
            rsp = String(id + "," + timeReceived_string + "," + localTime_string + ",");
            //Serial.println(rsp);
            rsp.toCharArray(RSP,32);
            Rad.transmit_data(RSP);
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}
