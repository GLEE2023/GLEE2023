// GLEE 2023 Module 2: Intro to Arduino - Hello Luna

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // This sets the baud rate
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello Luna!");  // This prints Hello Luna
  delay(5000); // This delays the loop for 5 seconds
}
