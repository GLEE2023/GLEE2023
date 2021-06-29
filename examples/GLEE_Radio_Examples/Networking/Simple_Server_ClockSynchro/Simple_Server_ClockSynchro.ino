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
char RSP[32];
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
    Serial.begin(9600);

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
    Serial.println(interruptEnabled);
}

void loop(){
    
    localTime = millis(); // Change to microseconds for different tests
    if(localTime % 1000 <= 100){ // Change to microseconds for different tests
        //Blink LED
        digitalWrite(LED2, HIGH);
        delay(100);
        digitalWrite(LED2, LOW);
    }
    if(messageRecieved){
        // Disable interrupts during reception processing
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        timeReceived = millis(); // Change to microseconds for different tests
        timeReceived_string = String(timeReceived);
        //Serial.print("Time Received: ");
        //Serial.println(timeReceived_string);

        digitalWrite(LED1, HIGH);
        delay(500);
        digitalWrite(LED1, LOW);
        
        

        //Read data from request
        char data_buffer[32];

        Rad.readData(data_buffer, 32);
        //String rqst((const __FlashStringHelper*) data_buffer);
        Serial.println(data_buffer);
        String head = String(data_buffer[0])+String(data_buffer[1]);
        //Serial.println(head);
        //If the request matches the time request flag, send back time received and time sent
        if(head=="R5"){
            Serial.println(F("Recieved request, sending response"));

            localTime = millis(); // Change to microseconds for different tests
            localTime_string = String(localTime);
            //Serial.println(localTime_string);
            rsp = "";
            rsp = String("R1," + timeReceived_string + "," + localTime_string + ",");
            rsp.toCharArray(RSP,32);
            Rad.transmit_data(RSP);
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
}