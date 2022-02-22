/* This sketch is just to test basic window transmit and receive functionality. */

/* This sketch is intended to turn the LunaSat into a Class B LoRa device, where packets
can only be received during two designated windows following transmission. Once 
gateway development is underway, this sketch may need to be modified so that packets
are readable by the gateway. For now, this sketch is intended to send and receive packets 
from another LunaSat. This sketch will focus on functionality correlated with Class A devices,
which makes up some of Class B functionality. */

#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

unsigned long windowOneStart = 0;
unsigned long windowOneEnd = 0;
unsigned long windowTwoStart = 0;
unsigned long windowTwoEnd = 0;
unsigned long windowLength = 1000;
unsigned long tmp = 0;
unsigned long classBWindows[6] = {1,10,20,30,40,50}; // This array will hold the start times of additional windows that the LunaSat
                                // can receive packets during (in minutes). These windows fall after the first two windows 
                                // following transmission. These windows would ideally be set by the gateway but they are
                                // hardcoded for now.

char RSP[5];
String rsp;
bool check = true;

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
    delay(50);
    Serial.println("I am the client.");
    strcpy(RSP, "Ping");
    Rad.transmit_data(RSP);
    tmp = millis();
    windowOneStart = tmp + 0; // Replace with appropriate constant
    windowTwoStart = tmp + 1000; // Replace with appropriate constant
    windowOneEnd = windowOneStart + windowLength; // Replace with length of window
    windowTwoEnd = windowTwoStart + windowLength; // Replace with length of window

}

void loop(){
      if(messageRecieved && (millis() > windowOneStart && millis() < windowOneEnd) || (millis() > windowTwoStart && millis() < windowTwoEnd) && check){
          // Disable interrupts during reception processing

          interruptEnabled = false;
          check = false;
          // reset reception flag 
          messageRecieved = false;

          Rad.readData(RSP, 5);

          rsp = "";
          for(int i = 0; i < 5; i++){
            rsp = rsp + RSP[i];
          }

          Serial.println(rsp);
          
          delay(50);
          // return to listening for transmissions 
          Rad.startRecieve();
          // we're ready to receive more packets,
          // enable interrupt service routine
          interruptEnabled = true;
          
      }
}