#ifndef _RADIOLIB_H
#define _RADIOLIB_H

/*!
  \mainpage RadioLib Documentation

  Universal wireless communication library for Arduino.

  \par Currently Supported Wireless Modules and Protocols
  - CC1101 FSK module
  - HC05 Bluetooth module
  - JDY08 BLE module
  - RF69 FSK module
  - Si443x FSK module
  - SX126x LoRa/FSK module
  - SX127x LoRa/FSK module
  - SX128x LoRa/GFSK/BLE/FLRC module
  - SX1231 FSK module
  - XBee module (S2B)
  - PhysicalLayer protocols
    - RTTY (RTTYClient)
    - Morse Code (MorseClient)
    - AX.25 (AX25Client)
    - SSTV (SSTVClient)
    - Hellschreiber (HellClient)
  - TransportLayer protocols
    - HTTP (HTTPClient)
    - MQTT (MQTTClient)

  \par Quick Links
  Documentation for most common methods can be found in its reference page (see the list above).\n
  Some methods (mainly configuration) are also overridden in derived classes, such as SX1272, SX1278, RFM96 etc. for SX127x.\n
  \ref status_codes have their own page.\n
  Some modules implement methods of one or more compatibility layers, loosely based on the ISO/OSI model:
  - PhysicalLayer - FSK and LoRa radio modules
  - TransportLayer - Modules with Internet connectivity

  \see https://github.com/jgromes/RadioLib

  \copyright  Copyright (c) 2019 Jan Gromes
*/

#include "TypeDef.h"
#include "Module.h"

// warnings are printed in this file since BuildOpt.h is compiled in multiple places

// check God mode
#ifdef RADIOLIB_GODMODE
  #warning "God mode active, I hope it was intentional. Buckle up, lads."
#endif

// print debug info
#ifdef RADIOLIB_DEBUG
  #pragma message "RADIOLIB_PLATFORM: " RADIOLIB_PLATFORM
#endif

// check unknown/unsupported platform
#ifdef RADIOLIB_UNKNOWN_PLATFORM
  #warning "RadioLib might not be compatible with this Arduino board - check supported platforms at https://github.com/jgromes/RadioLib!"
#endif

#include "modules/SX127x/SX1272.h"

// physical layer protocols
#include "protocols/PhysicalLayer/PhysicalLayer.h"
// // transport layer protocols
// #include "protocols/TransportLayer/TransportLayer.h"
// #include "protocols/HTTP/HTTP.h"
// #include "protocols/MQTT/MQTT.h"

// only create Radio class when using RadioShield
#ifdef RADIOLIB_RADIOSHIELD

// RadioShield pin definitions
#define RADIOSHIELD_CS_A   10
#define RADIOSHIELD_RX_A   9
#define RADIOSHIELD_TX_A   8
#define RADIOSHIELD_CS_B   5
#define RADIOSHIELD_RX_B   7
#define RADIOSHIELD_TX_B   6
#define RADIOSHIELD_INT_0  2
#define RADIOSHIELD_INT_1  3

/*!
  \class Radio

  \brief Library control object when using RadioShield.
  Contains two pre-configured "modules", which correspond to the slots on shield.
*/
class Radio {
  public:

    Module* ModuleA;
    Module* ModuleB;

    /*!
      \brief Default constructor. Only used to set ModuleA and ModuleB configuration.
    */
    Radio() {
      ModuleA = new Module(RADIOSHIELD_CS_A, RADIOSHIELD_INT_0, RADIOSHIELD_INT_1, RADIOSHIELD_RX_A, RADIOSHIELD_TX_A, SPI, SPISettings(2000000, MSBFIRST, SPI_MODE0), nullptr);
      ModuleB = new Module(RADIOSHIELD_CS_B, RADIOSHIELD_INT_0, RADIOSHIELD_INT_1, RADIOSHIELD_RX_B, RADIOSHIELD_TX_B, SPI, SPISettings(2000000, MSBFIRST, SPI_MODE0), nullptr);
    }

#ifndef RADIOLIB_GODMODE
  private:
#endif

};

Radio RadioShield;
#endif

#endif
