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
unsigned long localTime;
String localTime_string;

unsigned long timeClientReceived;

int timeServerReceived;
String timeServerRecieved_string;

unsigned long timeClientSent;

int timeServerSent;
String timeServerSent_string;

String sendID;

//Interval between synchronizations, 120000ms or 2 minutes by default
unsigned long interval = 120000;

//For storing output pin configuration of LEDs
int LED1 = 4; 
int LED2 = 5; 

void setup() {
    //Set the data rate to 9600 bits per second
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
    localTime = millis() + clockSkew; // Change to microseconds for different tests
    timeClientSent = millis() + clockSkew; // Change to microseconds for different tests
    rsp = String("R5");
    rsp.toCharArray(RSP,16);
    Rad.transmit_data(RSP); // Initial synchronization
}

void loop(){
    localTime = millis() + clockSkew;
    // Request a packet from the server ever hour
    if(localTime % interval <= 150){ // Adjust these values for different tests
        timeClientSent = millis() + clockSkew; // Change to microseconds for different tests

        rsp = String("R5");
        rsp.toCharArray(RSP,16);
        Rad.transmit_data(RSP);
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
        byte data_buffer[80];

        Rad.readData(data_buffer, 80);
        rqst = String((char*)data_buffer);

        sendID = rqst.substring(0,2);

        int i = rqst.indexOf(',',3);

        timeServerRecieved_string = rqst.substring(3,i);
        timeServerSent_string = rqst.substring(i+1);
        
        timeServerReceived = atol(timeServerRecieved_string.c_str());
        timeServerSent = atol(timeServerSent_string.c_str());

        if(sendID=="R1"){
            // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
            //Serial.println(F("Recieved request."));

            //Calculate clock skew
            unsigned long networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
	        float serverTimeWhenClientReceived = timeServerSent + (networkDelay/2);
	        clockSkew = serverTimeWhenClientReceived - timeClientReceived;

	        // Print clock skew
            //Serial.print("Clock Skew: "); Serial.print(clockSkew); Serial.println(" milliseconds");
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
    
}