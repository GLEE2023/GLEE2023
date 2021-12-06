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
char RSP[50];
String rsp;

unsigned long localTime; // This is also unsigned
//String localTime_string; 
char localTime_string[8] = {' '};

unsigned long timeReceived; // Check here first! This is unsigned here, but signed in the client code.
//String timeReceived_string;
char timeReceived_string[8] = {' '};


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
    localTime += (millis()-localTime); 
    if((localTime % 1000 > 540) && (localTime % 1000 < 545)){ 
        //Blink LED
        digitalWrite(LED2, HIGH);
    } else if ((localTime % 1000 > 740) && (localTime % 1000 < 745)){
        digitalWrite(LED2, LOW);
    }
    if(messageRecieved){
        localTime += (millis()-localTime);
        // Disable interrupts during reception processing
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;
        
        timeReceived = localTime; 
        
        //Read data from request
        Rad.readData(RSP, 50);
        
        String message_id = String(RSP[0])+String(RSP[1]);

        sprintf(timeReceived_string, "%lu", timeReceived);
        
        //If the request matches the time request flag, send back time received and time sent
        if(message_id=="L2"){
            rsp = "";
            Serial.println(F("Recieved request from LunaSat, sending response"));
            localTime += (millis()-localTime); // Change to microseconds for different tests
            sprintf(localTime_string, "%lu", localTime);
            strcpy(RSP, "L1");
            strcat(RSP,",");
            strcat(RSP,timeReceived_string);
            strcat(RSP,",");
            strcat(RSP,localTime_string);
            strcat(RSP,",");
            strcat(RSP,"|||");
   
            Rad.transmit_data(RSP);
            
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}