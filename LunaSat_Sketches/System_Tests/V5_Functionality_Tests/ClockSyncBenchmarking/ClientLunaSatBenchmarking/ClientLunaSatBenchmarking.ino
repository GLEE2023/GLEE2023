// disable interrupt when it's not needed
volatile bool interruptEnabled = true;

// Assemble LunaSat Packet
// Response char array and string variables
char RSP[16];
String rsp;

//Clock skew and time variables
unsigned long localTime;
String localTime_string;

unsigned long timeClientReceived;
String timeClientReceived_string;

unsigned long timeServerReceived;
String timeServerReceived_string;

unsigned long timeClientSent;
String timeClientSent_string;

unsigned long timeServerSent;
String timeServerSent_string;

long clockSkew = 0;

// Uncomment for Benchmarking Application 4
//long temperingConstant = 0;
//bool findConstant = false;

// Used to determine the time it takes to process a packet and calculate the clock skew
unsigned long processingTimeStart;
unsigned long processingTimeEnd;

// Use this to set the time interval at which clock synchronization will be performed in the appropriate units
long interval = 1800000000; // Currently set to 30 minutes in microseconds

String packet;

void setup() {
    // Set the data rate to 9600 bits per second
    Serial.begin(9600); // hardware serial we use to monitor communications

    while(!Serial){} // Wait for serial to initialize

    delay(1000);

    // Send initial synchronization request packet.
    timeClientSent = micros(); // Change for different tests [micros(), millis(), or millis()/1000 for seconds]
    Serial.print("syncRequest");
   
}

void loop(){
    // Request a packet from the server every [interval]
    if((micros() + clockSkew) % interval <= 50){
        timeClientSent = micros(); // Change for different tests [micros(), millis(), or millis()/1000 for seconds]
        Serial.print("syncRequest");
    }

    // Process packet from server (Lead LunaSat)
    if(Serial.available()){
        // Disable interrupts during reception processing
        timeClientReceived = micros(); // Change for different tests [micros(), millis(), or millis()/1000]
        processingTimeStart = micros(); // Change for different tests [micros(), millis(), or millis()/1000]
        interruptEnabled = false;

        //Serial.print("\nServer response received");

        //Get data from packet

        packet = Serial.readString();
        //Serial.print(", read");

        int i = packet.indexOf(',');
        //Serial.println(", and parsed");

        //Serial.println(packet);

        timeServerReceived_string = packet.substring(0,i);
        timeServerSent_string = packet.substring(i+1);

        timeServerReceived = atol(timeServerReceived_string.c_str());
        timeServerSent = atol(timeServerSent_string.c_str());

        // Calculate clock skew

        unsigned long networkDelay = (timeClientReceived - timeClientSent) - (timeServerSent - timeServerReceived);
        unsigned long serverTimeWhenClientReceived = timeServerSent + (networkDelay/2);

        clockSkew = serverTimeWhenClientReceived - timeClientReceived;
        
        processingTimeEnd = micros(); // Change for different tests [micros(), millis(), or millis()/1000]

        // Output clock skew with and without processing time

        Serial.print("Clock skew in microseconds: "); // Change units for different tests
        Serial.println(clockSkew);

        Serial.print("Clock skew in microseconds (w/ processing time factored in): "); //Change units for different tests
        Serial.println(clockSkew + (processingTimeEnd - processingTimeStart));


        // Uncomment for Benchmarking Application 4
        /*
        if(!findConstant){
            findConstant = true;
        } else {
            temperingConstant = (clockSkew / (interval/1000000)); // Make sure the correct units are being used, currently this is the constant for every second
            Serial.print("New tempering constant (in microseconds): ");
            Serial.println(temperingConstant);
        }*/
    }

    // we're ready to receive more packets,
    // enable interrupt service routine
    interruptEnabled = true;
}
