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

//Clock skew and time variables
unsigned long localTime;
String localTime_string;

unsigned long timeClientReceived;
String timeClientRecieved_string;

int timeServerReceived;
String timeServerRecieved_string;

unsigned long timeClientSent;
String timeClientSent_string;

int timeServerSent;
String timeServerSent_string;

String sendID;

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
    //Request a packet from the server ever hour
    if(millis() % 3600000){
        timeClientSent = millis();
        timeClientSent_string = String(timeClientSent);
        Rad.transmit_data("0");
    }
    //Process packet from server
    if(messageRecieved){
        // Disable interrupts during reception processing
        timeClientReceived = millis();
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        //Get times from packet

        byte data_buffer[24];

        Rad.readData(data_buffer, 24);
        rqst = String((char*)data_buffer);

        //Get data from packet
        int commaPosition = rqst.find(",");
        sendID = rqst.substring(0,commaPosition);
        int nextCommaPosition = rqst.find(",",commaPosition+1);
        timeServerRecieved_string = rqst.substring(commaPosition+1,nextCommaPosition);
        timeServerSent_string = rqst.substring(nextCommaPosition+1);
        
        timeServerReceived = stoi(timeServerRecieved_string);
        timeServerSent = stoi(timeServerSent_string);

        if(sendID==lunaSatID){
            // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
            Serial.println(F("Recieved request."));
            unsigned long networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
	        float serverTimeWhenClientReceived = timeServerSent + (networkDelay/2);
	        float clockSkew = serverTimeWhenClientReceived - timeClientReceived;
	        // Adjust clock accordingly.... 
            localTime = localTime + clockSkew
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}