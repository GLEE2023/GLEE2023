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

unsigned long localTime;
String localTime_string;

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
    localTime = micros();
}

void loop(){
  if(messageRecieved){
      // Disable interrupts during reception processing

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
          Serial.println(F("Recieved request, sending response"));

          localTime = micros();
          localTime_string = String(localTime);

          rsp = String("R" + lunaSatID + ","+ localTime_string);
          rsp.toCharArray(RSP,16);
          Rad.transmit_data(RSP);
      }

      // return to listening for transmissions 
      Rad.startRecieve();
      // we're ready to receive more packets,
      // enable interrupt service routine
      interruptEnabled = true;
  }
}