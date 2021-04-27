#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

//Lead LunaSat ID
String lunaSatID = "1";

//Lander ID
String landerID = "";

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

void setup() {
    //Set the data rate to 9600 bits pere second
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
    syncTime = getNtpTime();
    
}

void loop() {
    localTime = millis();
    //Request a packet from the server ever hour
    if(millis() % 3600000 || ((millis() > initialWaitTime) && (alreadySynchronized==false)){
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

        if(/* */){
            // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
            Serial.println(F("Recieved request."));

            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 =  (unsigned long)packetReceived[40] << 24;
            secsSince1900 |= (unsigned long)packetReceived[41] << 16;
            secsSince1900 |= (unsigned long)packetReceived[42] << 8;
            secsSince1900 |= (unsigned long)packetReceived[43];
            //syncTime = secsSince1900 - 2208988800UL;

            if(setTimeDirectly){
                localTime = syncTime;
                setTimeDirectly = false;
            } else {
                
              /*Calculate clock skew and adjust clock / clock rate
              //
              //
              */
            }

           /*Send accurate time to all other LunaSats
           //
           //
           */

          broadcastNTPPacket();
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
}



/*-------- NTP code ----------*/
//Similar to https://forum.arduino.cc/t/ntp-sntp-request/355504/7

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
    Serial.println("Transmit NTP Request");
    sendNTPpacketToLander(landerID);
}

// send an NTP request to the time server at the given address
void sendNTPpacketToLander(string landerID){
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 12;     // Polling Interval - 2^12 seconds is 4096 seconds, or about 1.14 hours
    packetBuffer[3] = ;  // Peer Clock Precision

    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;

    //Send NTP Request Packet
    Rad.transmit_data(packetBuffer);
}

void broadcastNTPpacket(){
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 1;     // Stratum, or type of clock
    packetBuffer[2] = 12;     // Polling Interval - 2^12 seconds is 4096 seconds, or about 1.14 hours
    packetBuffer[3] = ;  // Peer Clock Precision

    // 8 bytes of zero for Root Delay & Root Dispersion

    //Change these
    packetBuffer[12]  = 49; 
    packetBuffer[13]  = 0x4E; 
    packetBuffer[14]  = 49; 
    packetBuffer[15]  = 52;

    //Timestamps below (32 bytes)
    /*
    //
    */

    Rad.transmit_data(packetBuffer);
    
}


