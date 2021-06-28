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
char RSP[32];
String rsp;

float clockSkew = 0;

bool needClockSkew = false;

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

int timeServerReceived;
String timeServerRecieved_string;

unsigned long timeClientSent;

int timeServerSent;
String timeServerSent_string;

String sendID;

//Used to calculate the processing time for sending a packet
//long transmitAdditionalStartTime;
//long transmitAdditionalStopTime;

//Interval between synchronizations, 120000ms or 2 minutes by default
unsigned long interval = 10000;

//For storing output pin configuration of LEDs
int LED1 = 4; 
int LED2 = 5; 

void setup() {
    //Set the data rate to 9600 bits per second
    Serial.begin(9600);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    delay(2000);

    //Initialize the radio settings by using the initialize_radio function
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0,17,250.0,12,8);

    delay(5000);

    Rad.enable_recieve_interupt(recieve_callback);
    delay(1000);
    Serial.println(F("Initial sync"));
    localTime = millis() + clockSkew; // Change to microseconds for different tests
    timeClientSent = millis() + clockSkew; // Change to microseconds for different tests
    rsp = String("R5");
    rsp.toCharArray(RSP,32);
    Rad.transmit_data("R5"); // Initial synchronization
}

void loop(){
    localTime = millis() + clockSkew;
    // Request a packet from the server ever hour
    if(localTime % interval <= 150){ // Adjust these values for different tests
        timeClientSent = millis() + clockSkew; // Change to microseconds for different tests
        //transmitAdditionalStartTime = millis();
        rsp = String("R5");
        rsp.toCharArray(RSP,32);
        Rad.transmit_data(RSP);
        Serial.println(F("Sent message"));
        //transmitAdditionalStopTime = millis();
        digitalWrite(LED1, HIGH);
        delay(100);
        digitalWrite(LED1, LOW);
    } else if (localTime % 1000 <= 100){ // Change to microseconds for different tests
        //Blink LED
        digitalWrite(LED2, HIGH);
        delay(100);
        digitalWrite(LED2, LOW);
    }

    //Process packet from server
    if(messageRecieved){
        // Disable interrupts during reception processing
        timeClientReceived = millis() + clockSkew; // Change to microseconds for different tests
        interruptEnabled = false;

        digitalWrite(LED1, HIGH);
        delay(500);
        digitalWrite(LED1, LOW);

        // reset reception flag 
        messageRecieved = false;

        //Get data from packet
        char data_buffer[32];

        Rad.readData(data_buffer, 32);
        //rqst = String((char*)data_buffer);
        //Serial.println(messageString);
        String response = "";
        for(int i = 0; i < 32; i++){
          response = response + data_buffer[i];
        }
        Serial.println(response);
        String head = String(data_buffer[0])+String(data_buffer[1]);
        //Serial.println(sendID);
        
        int i = rqst.indexOf(',',3);
        int j = rqst.indexOf(',',i);

        timeServerRecieved_string = rqst.substring(3,i);
        timeServerSent_string = rqst.substring(i+1,j);
        
        timeServerReceived = atol(timeServerRecieved_string.c_str());
        timeServerSent = atol(timeServerSent_string.c_str());

        if(head=="R1"){
            // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
            Serial.println(F("Recieved request."));

            //Calculate clock skew
            unsigned long networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
	          float serverTimeWhenClientReceived = timeServerSent + (networkDelay/2);
            if(needClockSkew){
              clockSkew = timeClientReceived - serverTimeWhenClientReceived;
              Serial.print("Clock Skew: "); Serial.print(clockSkew); Serial.println(" milliseconds");
            } else {
              needClockSkew = true;
            }
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
    
}