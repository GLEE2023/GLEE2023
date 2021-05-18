#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

String rqst;

// Assemble LunaSat Packet
// Response char array and string variables
char RSP[80];
String rsp;

unsigned long localTime;
String localTime_string;

unsigned long timeReceived;
String timeReceived_string;

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
    //Serial.begin(9600);

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
}

void loop(){
    if(messageRecieved){
        // Disable interrupts during reception processing
        interruptEnabled = false;

        timeReceived = millis(); //Change to seconds or microseconds for different tests
        timeReceived_string = String(timeReceived);

        digitalWrite(LED1, HIGH);
        delay(500);
        digitalWrite(LED1, LOW);
        
        // reset reception flag 
        messageRecieved = false;

        //Read data from request
        byte data_buffer[16];

        Rad.readData(data_buffer, 16);
        rqst = String((char*)data_buffer);
        
        //If the request matches the time request flag, send back time received and time sent
        if(rqst=="R5"){
            //Serial.println(F("Recieved request, sending response"));

            localTime = millis(); //Change to seconds or microseconds for different tests
            localTime_string = String(localTime);

            rsp = String("R1," + timeReceived_string + "," + localTime_string);
            rsp.toCharArray(RSP,80);
            Rad.transmit_data(RSP);
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
    digitalWrite(LED2, HIGH);
    delay(100);
    digitalWrite(LED2, LOW);
}