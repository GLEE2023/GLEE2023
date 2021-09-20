// GLEE 2023 Module 2: Intro to Arduino - Blink Sketch

// Welcome to Module 2, let's get some lights blinking!

// We begin by defining variables LED1 and LED2 to store output pin configuration
int LED1 = 4;
int LED2 = 5;

// Next we define variables to configure time off and between blinks
int on_time = 1000;
int off_time = 1000;

void setup(){
    // With setup() only running onces per device reset, we utilize the pinMode function to set pins LED1 and LED2 as outputs
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
};

void loop(){
    // The code declared within an Arduino sketches loop function runs continuously following setup, 
    // so we'll use this to continuously turn both LEDS on and off with previously defined time periods 

    // DigitalWrite is used to set the LED1 and LED2 pins to a high voltage (3.3V) 
    // which will allow current to flow through the circuit and turn the LEDs on
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    
    // A call to the delay function after setting LED pins to high is used to control how long the LEDs stay on for
    delay(on_time);

    // Following the on_time delay, digitalWrite is used again to set LED pins to low causing the LEDs to turn off
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);

    // To finalize the loop sequence, a delay of off_time is implemented
    delay(off_time);
};
