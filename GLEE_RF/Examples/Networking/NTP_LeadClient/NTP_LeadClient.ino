#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

//Lead LunaSat ID
int lunaSatID = 1;

//Lander ID
String landerID = "";

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

bool alreadySynchronized = false; //False on wakeup, true after wait time is over
bool setTimeDirectly = true; //True for first synchronization, false otherwise

char RSP[24];
String rsp;

//Wait time (in milliseconds) before first synchronization after wakeup
int initialWaitTime = 10000;

// Time variables

unsigned long localTime;
String localTime_string;

unsigned long syncTime;
String syncTime_string;



/*-------- NTP code ----------*/
//Similar to https://forum.arduino.cc/t/ntp-sntp-request/355504/7

const int NTP_PACKET_SIZE = 36; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

void getNtpTime(void){
    Serial.println("Transmit NTP Request");
    sendNTPpacketToLander(landerID);
}

// send an NTP request to the time server at the given address
void sendNTPpacketToLander(String landerID){
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    packetBuffer[0] = 011;   // Mode (3 for client)
    packetBuffer[1] = 1;     // Stratum, or type of clock
    packetBuffer[2] = lunaSatID;    // THIS LunaSat's ID, not a typical NTP field

    //Timestamps below (32 bytes)

    packetBuffer[4] = 0;     //Reference Timestamp - last time clock was corrected 
    packetBuffer[12] = 0;    //Originate Timestamp - time this request is sent
    packetBuffer[20] = 0;    //Receive Timestamp - time server receives request (should be blank here)
    packetBuffer[28] = 0;    //Transmit Timestamp - time request departs server (should be blank here)


    //Send NTP Request Packet
    Rad.transmit_data(packetBuffer);
}

void broadcastNTPpacket(void){
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    packetBuffer[0] = 101;   // Mode (5 for broadcast)
    packetBuffer[1] = 1;     // Stratum, or type of clock
    packetBuffer[2] = lunaSatID;    // THIS LunaSat's ID, not a typical NTP field

    //Timestamps below (x bytes)

    packetBuffer[4] = 0; //Reference Timestamp (last time clock was corrected)
    packetBuffer[28] = 0; //Transmit Timestamp (time at which broadcast will be sent out)

    Rad.transmit_data(packetBuffer);
    
}






void recieve_callback(void) {
    // don't set flag if interrupt isn't enabled
    if(!interruptEnabled) {
      return;
    }

    // set flag signifying message was recieved
    messageRecieved = true;
}

void setup() {
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
    getNtpTime();
    
}

void loop() {
    localTime = millis();
    //Request a packet from the server ever hour, or after wait time is completed after wakeup
    if((millis() % 3600000) || ((millis() > initialWaitTime) && (!alreadySynchronized))){
        alreadySynchronized = true;
        getNtpTime();
    }
    if(messageRecieved){
        // Disable interrupts during reception processing
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        //Get times from packet

        byte packetReceived[48];

        Rad.readData(packetReceived, 48);
      
        //Get data from packet
        /*
        //
        //
        */

        if(rsp == "..."){ // TODO
            // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
            Serial.println(F("Recieved request."));

            //unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            /*
            secsSince1900 =  (unsigned long)packetReceived[40] << 24;
            secsSince1900 |= (unsigned long)packetReceived[41] << 16;
            secsSince1900 |= (unsigned long)packetReceived[42] << 8;
            secsSince1900 |= (unsigned long)packetReceived[43];
            */
            //syncTime = secsSince1900 - 2208988800UL;

            if(!setTimeDirectly){
                localTime = syncTime;
                setTimeDirectly = false;
            } else { 
                /*Calculate clock skew and adjust clock / clock rate
                //
                //
                */
            }

            //Send accurate time to all other LunaSats
            broadcastNTPpacket();
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}


