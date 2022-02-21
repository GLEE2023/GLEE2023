/* This sketch is intended to turn the LunaSat into a Class B LoRa device, where packets
can only be received during two designated windows following transmission. Once 
gateway development is underway, this sketch may be discarded as the gateway may not support
this code at all. For now, this sketch is intended to be run on a server LunaSat that will
pretend to be the gateway device. */

#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

unsigned long classBWindows[6] = {1,10,20,30,40,50};

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
}

void loop(){
    if(messageRecieved){
        // Disable interrupts during reception processing

        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        String recieved_msg = Rad.receive_data_string();
        
        // print data of the packet
        Serial.println(recieved_msg);

        Rad.transmit_data(String(classBWindows[0] + "," + classBWindows[1] + "," + classBWindows[2] + "," + classBWindows[3] + "," + classBWindows[4] + "," + classBWindows[5] + ","));
        delay(10);
        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}