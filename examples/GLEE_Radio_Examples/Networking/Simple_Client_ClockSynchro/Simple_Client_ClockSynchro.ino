#include <GLEE_Radio.h>
#include <stdio.h>
#include <stdlib.h>

//Initialize RF Object
LunaRadio Rad;

String lunaSatID = "1";

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

// Assemble LunaSat Packet
// Response char array and string variables
char RSP[32];
String rsp;

float clockSkew = 0;

void recieve_callback(void) {
  // don't set flag if interrupt isn't enabled
  if(!interruptEnabled) {
    return;
  }

  // set flag signifying message was recieved
  messageRecieved = true;
}

//Clock skew and time variables
long localTime;
String localTime_string;

long timeClientReceived;

long timeServerReceived;
String timeServerRecieved_string;

long timeClientSent;

long timeServerSent;
String timeServerSent_string;

long timeSinceLastUpdate;

//Interval between synchronizations, 120000ms or 2 minutes by default
unsigned long interval = 120000;

//For storing output pin configuration of LEDs
int LED1 = 4; 
int LED2 = 5; 

void setup() {
    //Set the data rate to 9600 bits per second
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
    Serial.println(F("Initial sync"));
    localTime = millis(); // Change to microseconds for different tests
    timeClientSent = millis(); // Change to microseconds for different tests
    rsp = String("R5");
    rsp.toCharArray(RSP,32);
    Rad.transmit_data("R5"); // Initial synchronization
}

void loop(){
    timeSinceLastUpdate = millis() - localTime;
    localTime += ((millis()-localTime) + (int)clockSkew);
    // Request a packet from the server every interval
    if(localTime % interval <= 150){ // Adjust these values for different tests
        timeSinceLastUpdate = millis() - localTime;
        localTime += ((millis()-localTime) + (int)clockSkew);
        timeClientSent = localTime; // Change to microseconds for different tests
        rsp = String("R5");
        rsp.toCharArray(RSP,32);
        Rad.transmit_data(RSP);
        Serial.println(F("Sent message"));
        interval = 120000;
    } else if ((localTime % 1000 <= 5)){ // Change to microseconds for different tests
        //Blink LED
        digitalWrite(LED2, HIGH);
    } else if ((localTime % 1000 > 110) && (localTime % 1000 < 115)){
        digitalWrite(LED2, LOW);
    }

    //Process packet from server
    if(messageRecieved){
        // Disable interrupts during reception processing
        timeSinceLastUpdate = millis() - localTime;
        localTime += ((millis()-localTime) + (int)clockSkew);
        timeClientReceived = localTime; // Change to microseconds for different tests
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        //Get data from packet
        char data_buffer[32];

        Rad.readData(data_buffer, 32);
        String response = "";
        for(int i = 0; i < 32; i++){
          response = response + data_buffer[i];
        }
        //Serial.println(response);
        String head = String(data_buffer[0])+String(data_buffer[1]);
        
        int i = response.indexOf(',',3);
        int j = response.indexOf(',',i+1);

        timeServerRecieved_string = response.substring(3,i);
        timeServerSent_string = response.substring(i+1,j);
        timeServerReceived = atol(timeServerRecieved_string.c_str());
        timeServerSent = atol(timeServerSent_string.c_str());

        if(head=="R1"){
            // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
            Serial.println(timeClientSent);
            Serial.println(timeServerReceived);
            Serial.println(timeServerSent);
            Serial.println(timeClientReceived);
            //Calculate clock skew
            long networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
	          float serverTimeWhenClientReceived = timeServerSent + (networkDelay/2);
            clockSkew += (serverTimeWhenClientReceived - timeClientReceived + 200);
            float change = (serverTimeWhenClientReceived - timeClientReceived + 200);
            Serial.print("Cumulative Clock Skew: "); Serial.print(clockSkew); Serial.println(" milliseconds");
            Serial.print("Change in Clock Skew: "); Serial.print(change); Serial.println(" milliseconds");
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
    
}