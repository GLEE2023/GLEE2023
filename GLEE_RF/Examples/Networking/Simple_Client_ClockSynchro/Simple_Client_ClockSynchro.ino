#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

String lunaSatID = "1";

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

// Assemble LunaSat Packet
String rqst;

// Assemble LunaSat Packet
// Response char array and string variables
char RSP[16];
String rsp;

void recieve_callback(void) {
  // don't set flag if interrupt isn't enabled
  if(!interruptEnabled) {
    return;
  }

  // set flag signifying message was recieved
  messageRecieved = true;
}

//Clock Slew Variables
unsigned long localTime;
String localTime_string;
unsigned long timeClientReceived;
String timeClientRecieved_string;
unsigned long timeServerReceived;
String timeServerRecieved_string;
unsigned long timeClientSent;
String timeClientSent_string;
unsigned long timeServerSent;
String timeServerSent_string;

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
}

void loop(){
    if(millis() % 3600000){
        timeClientSent = millis();
        timeClientSent_string = String(timeClientSent);

        rsp = String("R" + "...ID..." + ","+ timeClientSent_string);
        rsp.toCharArray(RSP,16);
        Rad.transmit_data(RSP);
    }
    if(messageRecieved){
        // Disable interrupts during reception processing
        timeReceived = millis();
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        byte data_buffer[8];

        Rad.readData(data_buffer, 8);
        rqst = String((char*)data_buffer);
        // print data of the packet
        Serial.print(F("Recieved Request:\t\t"));
        Serial.println(rqst);
        
        if(rqst==lunaSatID){
            // If the data_buffer is the lunasat ID, then respond to the request
            Serial.println(F("Recieved request."));
            float clockSlew = estimateClockSlew(time1,time2,time3,timeReceived);
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}