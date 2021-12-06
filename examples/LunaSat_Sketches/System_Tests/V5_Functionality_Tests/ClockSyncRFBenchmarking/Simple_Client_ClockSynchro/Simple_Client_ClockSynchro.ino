#include <GLEE_Radio.h>
#include <stdio.h>
#include <stdlib.h>

//Initialize RF Object
LunaRadio Rad;

String id = "L2"; 
String serverid = "L1"; 

// flag to indicate that a packet was received
volatile bool messageRecieved = false;

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

// Response char array and string variables
char RSP[50];
String rsp;

// Sync Algorithm variables
long clockSkew = 0;
unsigned long serverTimeWhenClientReceived;
long networkDelay;

//Clock skew and time variables
unsigned long localTime;
String localTime_string;
char timeStamp_str[8] = {' '};

unsigned long timeClientReceived;

unsigned long timeServerReceived;
String timeServerRecieved_string;


unsigned long timeClientSent;

unsigned long timeServerSent;
String timeServerSent_string;

//Interval between synchronizations, 120000ms or 2 minutes by default
unsigned long interval = 1000;

//For storing output pin contfiguration of LEDs
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
    Serial.println(interruptEnabled);

    Serial.println(F("Initial sync"));
    timeClientSent = millis(); 
    strcpy(RSP, "L2,");
    strcat(RSP,"|||");
    Rad.transmit_data(RSP); // Initial synchronization
}

void loop(){
    localTime += ((millis()-localTime) + (long)clockSkew);
    // Request a packet from the server every interval
    if ((localTime % 1000 > 340) && (localTime % 1000 < 345)){
        //Blink LED
        digitalWrite(LED2, HIGH);
    } else if ((localTime % 1000 > 540) && (localTime % 1000 < 545)){
        digitalWrite(LED2, LOW);
    } else if(localTime % interval <= 150){ 
        localTime += ((millis()-localTime) + (long)clockSkew);
        timeClientSent = localTime; 
        strcpy(RSP, "L2,");
        strcat(RSP,"|||");
        Rad.transmit_data(RSP);
        Serial.println(F("Sent message"));
        interval = 20000;
    }
    //Process packet from server
    
    if(messageRecieved){
        // Disable interrupts during reception processing
        localTime += ((millis()-localTime) + (long)clockSkew);
        timeClientReceived = localTime; 
        interruptEnabled = false;

        // reset reception flag 
        messageRecieved = false;

        //Get data from packet
        char response[50];
        Rad.readData(RSP, 50);

        rsp = "";
        for(int i = 0; i < 50; i++){
          rsp = rsp + RSP[i];
        }

        String message_id = String(RSP[0])+String(RSP[1]);
        
        int i = rsp.indexOf(',',3);
        int j = rsp.indexOf(',',i+1);

        timeServerRecieved_string = rsp.substring(3,i);
        timeServerSent_string = rsp.substring(i+1,j);
        timeServerReceived = atol(timeServerRecieved_string.c_str());
        timeServerSent = atol(timeServerSent_string.c_str());
        // If the data_buffer is the lunasat ID, then use the times in the packet to calculate the clock skew
        if(message_id=="L1"){
            //Calculate clock skew
            networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
            serverTimeWhenClientReceived = timeServerSent + (long)(networkDelay/2);

            Serial.print(rsp);
            Serial.println();
            
            clockSkew += (serverTimeWhenClientReceived - timeClientReceived);
            Serial.print("Cumulative Clock Skew: "); Serial.print(clockSkew); Serial.println(" milliseconds");
        }

        // return to listening for transmissions 
        Rad.startRecieve();
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
    }
    
}