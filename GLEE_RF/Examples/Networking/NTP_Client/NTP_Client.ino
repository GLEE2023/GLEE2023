#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

//Lead LunaSat ID
String lunaSatID = "999";

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

bool alreadySynchronized = false;
bool setTimeDirectly = true;

char RSP[24];
String rsp;


// Time variables

unsigned long localTime;
String localTime_string;

unsigned long syncTime;
String syncTime_string;



void recieve_callback(void) {
    // don't set flag if interrupt isn't enabled
    if(!interruptEnabled) {
      return;
    }

    // set flag signifying message was recieved
    messageRecieved = true;
}

void setup(){
    //Set the data rate to 9600 bits per second
    Serial.begin(9600);

    //Initialize the radio settings by using the initialize_radio function
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0,17,250.0,12,8);

    Rad.enable_recieve_interupt(recieve_callback);
    localTime = millis();
}

void loop(){

}



/**
 * Parameters: None
 * Returns: None
 * This function handle synchronization upon wakeup.
**/
void uponWakeup(){

}

/**
 * Parameters: None
 * Returns: None
 * This function handle finding a path to the lead if unable to directly communicate.
**/
void findRepeater(){

}





