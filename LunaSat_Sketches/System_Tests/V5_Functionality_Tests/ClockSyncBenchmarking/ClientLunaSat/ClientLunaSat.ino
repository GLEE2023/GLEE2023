//#include <SoftwareSerial.h>

// disable interrupt when it's not needed
volatile bool interruptEnabled = true;
​
// Assemble LunaSat Packet
// Response char array and string variables
char RSP[16];
String rsp;
​
//Clock skew and time variables
unsigned long localTime;
String localTime_string;
​
unsigned long timeClientReceived;
String timeClientRecieved_string;
​
unsigned long timeServerReceived;
String timeServerRecieved_string;
​
unsigned long timeClientSent;
String timeClientSent_string;
​
unsigned long timeServerSent;
String timeServerSent_string;
​
long clockSkew = 0;
​
String packet;
​
void setup() {
    //Set the data rate to 9600 bits per second
    Serial.begin(9600); // hardware serial we use to monitor communications
    while(!Serial){}
}
​
void loop(){
    localTime = micros();// + clockSkew;
    
    //Request a packet from the server every 10 seconds (+/- 100 us)
    if(localTime % 4000000 <= 50){
        timeClientSent = micros();// + clockSkew; //Change to seconds or milliseconds for different tests
        Serial.print("syncRequest"); // send request packet
    }
​
​
    //Process packet from server
    if(Serial.available()){
        // Disable interrupts during reception processing
        timeClientReceived = micros();// + clockSkew; //Change to seconds or microseconds for different tests
        interruptEnabled = false;
​
        //Serial.print("\nServer response recieved");
​
        //Get data from packet
        packet = Serial.readString();
        //Serial.print(", read");
​
        int i = packet.indexOf(',');
        //Serial.println(", and parsed");
​
        //Serial.println(packet);
        
        timeServerRecieved_string = packet.substring(0,i);
        timeServerSent_string = packet.substring(i+1);
        
        timeServerReceived = atol(timeServerRecieved_string.c_str());
        timeServerSent = atol(timeServerSent_string.c_str());
​
​
        unsigned long networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
        unsigned long serverTimeWhenClientReceived = timeServerSent + (networkDelay/2);
​
        clockSkew = serverTimeWhenClientReceived - timeClientReceived;
​
        Serial.print("\n"); //Serial.print(clockSkew); Serial.println(" us\n");
        Serial.println(clockSkew);
    }
​
        // we're ready to receive more packets,
        // enable interrupt service routine
        interruptEnabled = true;
}