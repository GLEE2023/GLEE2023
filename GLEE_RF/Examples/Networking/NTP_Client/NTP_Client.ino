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

//Array of LunaSat IDs for rebroadcasts
int ids [100];

//ID of LunaSat being used as repeater
int repeaterID = -1;

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
    uponWakeup();
}

void loop(){

}



/**
 * Parameters: None
 * Returns: None
 * This function handles synchronization upon wakeup.
**/
void uponWakeup(void){

}

/**
 * Parameters: clock skew, interval between synchronizations
 * Returns: Adjustment constant
 * This function looks at clock drift and produces an constant to be added to the 
 * localTime on an interval to automatically account for clock drift.
**/
int autoTemperClock(long clockSkew, unsigned long interval){
  return (clockSkew / interval);
}

/**
 * Parameters: None
 * Returns: None
 * This function handles rebroadcasting a synchronizating broadcast if this LunaSat is a repeater for another LunaSat.
**/
void rebroadcast(){
    
}







/*-------- NTP code ----------*/
// Similar to https://forum.arduino.cc/t/ntp-sntp-request/355504/7

const int NTP_PACKET_SIZE = 36; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming & outgoing packets

void broadcastNTPpacket(){
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    packetBuffer[0] = 101;   // Mode (5 for broadcast)
    packetBuffer[1] = 2;     // Stratum, or type of clock
    pakcetBuffer[2] = lunaSatID;   // THIS LunaSat's ID, not a typical NTP field

    //Timestamps below (x bytes)

    packetBuffer[4] = ; // Reference Timestamp (last time clock was corrected)
    packetBuffer[28] = ; // Transmit Timestamp (time at which broadcast will be sent out)

    Rad.transmit_data(packetBuffer);
    
}

/**
 * Parameters: None
 * Returns: None
 * This function handles sending out an occassional ping to check for other available LunaSats nearby
**/
void broadcastSOS(void){
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    packetBuffer[0] = 111;   // Mode (7 for SOS)
    packetBuffer[1] = 2;     // Stratum, or type of clock
    pakcetBuffer[2] = lunaSatID;   // THIS LunaSat's ID, not a typical NTP field

    Rad.transmit_data(packetBuffer);
}

/**
 * Parameters: None
 * Returns: None
 * This function handles finding a path to the lead if unable to directly communicate.
**/
void findRepeater(void){

}





