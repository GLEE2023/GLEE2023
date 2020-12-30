// Hello LoRa - ABP TTN Packet Sender (Multi-Channel)
// Tutorial Link: https://learn.adafruit.com/the-things-network-for-feather/using-a-feather-32u4
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Copyright 2015, 2016 Ideetron B.V.
//
// Modified by Brent Rubell for Adafruit Industries, 2018
/************************** Configuration ***********************************/
#include <TinyLoRa.h>
#include <SPI.h>

// Visit your thethingsnetwork.org device console
// to create an account, or if you need your session keys.

// Network Session Key (MSB)
uint8_t NwkSkey[16] ={ 0x2C, 0xB2, 0x73, 0xEE, 0xA9, 0x2F, 0xEB, 0x99, 0x31, 0xC5, 0x34, 0x62, 0x83, 0x06, 0x8F, 0xAE };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { 0x21, 0x04, 0x3E, 0x54, 0x7E, 0x6F, 0xA6, 0x32, 0xCB, 0xE3, 0xF4, 0x9D, 0x64, 0xD7, 0x66, 0x72 };

// Device Address (MSB)
uint8_t DevAddr[4] = { 0x26, 0x02, 0x15, 0xE3 };

/************************** Example Begins Here ***********************************/
// Data Packet to Send to TTN
unsigned char loraData[11] = {"hello LoRa"};

// How many times data transfer should occur, in seconds
const unsigned int sendInterval = 30;

// Pinout for Adafruit Feather 32u4 LoRa
TinyLoRa lora = TinyLoRa(3, 10, 9);

// Pinout for Adafruit Feather M0 LoRa
//TinyLoRa lora = TinyLoRa(3, 8, 4);

void setup()
{
  Serial.begin(9600);
  while (! Serial);
  
  // Initialize pin LED_BUILTIN as an output
  pinMode(13, OUTPUT);
  
  // Initialize LoRa
  Serial.print("Starting LoRa...");
  // define multi-channel sending
  lora.setChannel(MULTI);
  // set datarate
  lora.setDatarate(SF7BW125);
  if(!lora.begin())
  {
    Serial.println("Failed");
    Serial.println("Check your radio");
    while(true);
  }

  // Optional set transmit power. If not set default is +17 dBm.
  // Valid options are: -80, 1 to 17, 20 (dBm).
  // For safe operation in 20dBm: your antenna must be 3:1 VWSR or better
  // and respect the 1% duty cycle.

  // lora.setPower(17);

  Serial.println("OK");
}

void loop()
{
  Serial.println("Sending LoRa Data...");
  lora.sendData(loraData, sizeof(loraData), lora.frameCounter);
  // Optionally set the Frame Port (1 to 255)
  // uint8_t framePort = 1;
  // lora.sendData(loraData, sizeof(loraData), lora.frameCounter, framePort);
  Serial.print("Frame Counter: ");Serial.println(lora.frameCounter);
  lora.frameCounter++;

  // blink LED to indicate packet sent
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  
  Serial.println("delaying...");
  delay(sendInterval * 1000);
}
