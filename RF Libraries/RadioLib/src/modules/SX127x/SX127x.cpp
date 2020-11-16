#include "SX127x.h"
#if !defined(RADIOLIB_EXCLUDE_SX127X)

SX127x::SX127x(Module* mod) : PhysicalLayer(SX127X_FREQUENCY_STEP_SIZE, SX127X_MAX_PACKET_LENGTH) {
  _mod = mod;
}

int16_t SX127x::begin(uint8_t chipVersion, uint8_t syncWord, uint16_t preambleLength) {
  // set module properties
  _mod->init(RADIOLIB_USE_SPI);
  Module::pinMode(_mod->getIrq(), INPUT);
  Module::pinMode(_mod->getGpio(), INPUT);

  // try to find the SX127x chip
  if(!SX127x::findChip(chipVersion)) {
    RADIOLIB_DEBUG_PRINTLN(F("No SX127x found!"));
    _mod->term(RADIOLIB_USE_SPI);
    return(ERR_CHIP_NOT_FOUND);
  } else {
    RADIOLIB_DEBUG_PRINTLN(F("Found SX127x!"));
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // check active modem
  if(getActiveModem() != SX127X_LORA) {
    // set LoRa mode
    state = setActiveModem(SX127X_LORA);
    RADIOLIB_ASSERT(state);
  }

  // set LoRa sync word
  state = SX127x::setSyncWord(syncWord);
  RADIOLIB_ASSERT(state);

  // set over current protection
  state = SX127x::setCurrentLimit(60);
  RADIOLIB_ASSERT(state);

  // set preamble length
  state = SX127x::setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  // initialize internal variables
  _dataRate = 0.0;

  return(state);
}

int16_t SX127x::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  int16_t modem = getActiveModem();
  uint32_t start = 0;
  if(modem == SX127X_LORA) {
    // calculate timeout (150 % of expected time-one-air)
    float symbolLength = (float)(uint32_t(1) <<_sf) / (float)_bw;
    float de = 0;
    if(symbolLength >= 16.0) {
      de = 1;
    }
    float ih = (float)_mod->SPIgetRegValue(SX127X_REG_MODEM_CONFIG_1, 0, 0);
    float crc = (float)(_mod->SPIgetRegValue(SX127X_REG_MODEM_CONFIG_2, 2, 2) >> 2);
    float n_pre = (float)((_mod->SPIgetRegValue(SX127X_REG_PREAMBLE_MSB) << 8) | _mod->SPIgetRegValue(SX127X_REG_PREAMBLE_LSB));
    float n_pay = 8.0 + max(ceil((8.0 * (float)len - 4.0 * (float)_sf + 28.0 + 16.0 * crc - 20.0 * ih)/(4.0 * (float)_sf - 8.0 * de)) * (float)_cr, 0.0);
    uint32_t timeout = ceil(symbolLength * (n_pre + n_pay + 4.25) * 1500.0);

    // start transmission
    state = startTransmit(data, len, addr);
    RADIOLIB_ASSERT(state);

    // wait for packet transmission or timeout
    start = Module::micros();
    while(!Module::digitalRead(_mod->getIrq())) {
      Module::yield();
      if(Module::micros() - start > timeout) {
        clearIRQFlags();
        return(ERR_TX_TIMEOUT);
      }
    }

  } else if(modem == SX127X_FSK_OOK) {
     RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  } else {
    return(ERR_UNKNOWN);
  }

  // update data rate
  uint32_t elapsed = Module::micros() - start;
  _dataRate = (len*8.0)/((float)elapsed/1000000.0);

  // clear interrupt flags
  clearIRQFlags();

  // set mode to standby to disable transmitter
  return(standby());
}

int16_t SX127x::receive(uint8_t* data, size_t len) {
  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  int16_t modem = getActiveModem();
  if(modem == SX127X_LORA) {
    // set mode to receive
    state = startReceive(len, SX127X_RXSINGLE);
    RADIOLIB_ASSERT(state);

    // wait for packet reception or timeout (100 LoRa symbols)
    while(!Module::digitalRead(_mod->getIrq())) {
      Module::yield();
      if(Module::digitalRead(_mod->getGpio())) {
        clearIRQFlags();
        return(ERR_RX_TIMEOUT);
      }
    }

  } else if(modem == SX127X_FSK_OOK) {
    RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

  // read the received data
  state = readData(data, len);

  return(state);
}

int16_t SX127x::scanChannel() {
  // check active modem
  if(getActiveModem() != SX127X_LORA) {
    return(ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // set DIO pin mapping
  state = _mod->SPIsetRegValue(SX127X_REG_DIO_MAPPING_1, SX127X_DIO0_CAD_DONE | SX127X_DIO1_CAD_DETECTED, 7, 4);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  clearIRQFlags();

  // set RF switch (if present)
  _mod->setRfSwitchState(HIGH, LOW);

  // set mode to CAD
  state = setMode(SX127X_CAD);
  RADIOLIB_ASSERT(state);

  // wait for channel activity detected or timeout
  while(!Module::digitalRead(_mod->getIrq())) {
    Module::yield();
    if(Module::digitalRead(_mod->getGpio())) {
      clearIRQFlags();
      return(PREAMBLE_DETECTED);
    }
  }

  // clear interrupt flags
  clearIRQFlags();

  return(CHANNEL_FREE);
}

int16_t SX127x::sleep() {
  // set RF switch (if present)
  _mod->setRfSwitchState(LOW, LOW);

  // set mode to sleep
  return(setMode(SX127X_SLEEP));
}

int16_t SX127x::standby() {
  // set RF switch (if present)
  _mod->setRfSwitchState(LOW, LOW);

  // set mode to standby
  return(setMode(SX127X_STANDBY));
}


int16_t SX127x::startReceive(uint8_t len, uint8_t mode) {
  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  int16_t modem = getActiveModem();
  if(modem == SX127X_LORA) {
    // set DIO pin mapping
    state = _mod->SPIsetRegValue(SX127X_REG_DIO_MAPPING_1, SX127X_DIO0_RX_DONE | SX127X_DIO1_RX_TIMEOUT, 7, 4);

    // set expected packet length for SF6
    if(_sf == 6) {
      state |= _mod->SPIsetRegValue(SX127X_REG_PAYLOAD_LENGTH, len);
    }

    // clear interrupt flags
    clearIRQFlags();

    // set FIFO pointers
    state |= _mod->SPIsetRegValue(SX127X_REG_FIFO_RX_BASE_ADDR, SX127X_FIFO_RX_BASE_ADDR_MAX);
    state |= _mod->SPIsetRegValue(SX127X_REG_FIFO_ADDR_PTR, SX127X_FIFO_RX_BASE_ADDR_MAX);
    RADIOLIB_ASSERT(state);

  } else if(modem == SX127X_FSK_OOK) {
    RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

  // set RF switch (if present)
  _mod->setRfSwitchState(HIGH, LOW);

  // set mode to receive
  return(setMode(mode));
}

void SX127x::setDio0Action(void (*func)(void)) {
  Module::attachInterrupt(RADIOLIB_DIGITAL_PIN_TO_INTERRUPT(_mod->getIrq()), func, RISING);
}

void SX127x::clearDio0Action() {
  Module::detachInterrupt(RADIOLIB_DIGITAL_PIN_TO_INTERRUPT(_mod->getIrq()));
}

void SX127x::setDio1Action(void (*func)(void)) {
  if(_mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  Module::attachInterrupt(RADIOLIB_DIGITAL_PIN_TO_INTERRUPT(_mod->getGpio()), func, RISING);
}

void SX127x::clearDio1Action() {
  if(_mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  Module::detachInterrupt(RADIOLIB_DIGITAL_PIN_TO_INTERRUPT(_mod->getGpio()));
}

int16_t SX127x::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);

  int16_t modem = getActiveModem();
  if(modem == SX127X_LORA) {
    // check packet length
    if(len >= SX127X_MAX_PACKET_LENGTH) {
      return(ERR_PACKET_TOO_LONG);
    }

    // set DIO mapping
    _mod->SPIsetRegValue(SX127X_REG_DIO_MAPPING_1, SX127X_DIO0_TX_DONE, 7, 6);

    // clear interrupt flags
    clearIRQFlags();

    // set packet length
    state |= _mod->SPIsetRegValue(SX127X_REG_PAYLOAD_LENGTH, len);

    // set FIFO pointers
    state |= _mod->SPIsetRegValue(SX127X_REG_FIFO_TX_BASE_ADDR, SX127X_FIFO_TX_BASE_ADDR_MAX);
    state |= _mod->SPIsetRegValue(SX127X_REG_FIFO_ADDR_PTR, SX127X_FIFO_TX_BASE_ADDR_MAX);

  } else if(modem == SX127X_FSK_OOK) {
    RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

  // write packet to FIFO
  _mod->SPIwriteRegisterBurst(SX127X_REG_FIFO, data, len);

  // set RF switch (if present)
  _mod->setRfSwitchState(LOW, HIGH);

  // start transmission
  state |= setMode(SX127X_TX);
  RADIOLIB_ASSERT(state);

  return(ERR_NONE);
}

int16_t SX127x::readData(uint8_t* data, size_t len) {
  int16_t modem = getActiveModem();
  size_t length = len;

  // put module to standby
  standby();

  if(modem == SX127X_LORA) {
    // len set to maximum indicates unknown packet length, read the number of actually received bytes
    if(len == SX127X_MAX_PACKET_LENGTH) {
      length = getPacketLength();
    }

    // check integrity CRC
    if(_mod->SPIgetRegValue(SX127X_REG_IRQ_FLAGS, 5, 5) == SX127X_CLEAR_IRQ_FLAG_PAYLOAD_CRC_ERROR) {
      // clear interrupt flags
      clearIRQFlags();

      return(ERR_CRC_MISMATCH);
    }

  } else if(modem == SX127X_FSK_OOK) {RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

  // read packet data
  _mod->SPIreadRegisterBurst(SX127X_REG_FIFO, length, data);

  // dump bytes that weren't requested
  size_t packetLength = getPacketLength();
  if(packetLength > length) {
    clearFIFO(packetLength - length);
  }

  // clear internal flag so getPacketLength can return the new packet length
  _packetLengthQueried = false;

  // clear interrupt flags
  clearIRQFlags();

  return(ERR_NONE);
}

int16_t SX127x::setSyncWord(uint8_t syncWord) {
  // check active modem
  if(getActiveModem() != SX127X_LORA) {
    return(ERR_WRONG_MODEM);
  }

  // set mode to standby
  setMode(SX127X_STANDBY);

  // write register
  return(_mod->SPIsetRegValue(SX127X_REG_SYNC_WORD, syncWord));
}

int16_t SX127x::setCurrentLimit(uint8_t currentLimit) {
  // check allowed range
  if(!(((currentLimit >= 45) && (currentLimit <= 240)) || (currentLimit == 0))) {
    return(ERR_INVALID_CURRENT_LIMIT);
  }

  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);

  // set OCP limit
  uint8_t raw;
  if(currentLimit == 0) {
    // limit set to 0, disable OCP
    state |= _mod->SPIsetRegValue(SX127X_REG_OCP, SX127X_OCP_OFF, 5, 5);
  } else if(currentLimit <= 120) {
    raw = (currentLimit - 45) / 5;
    state |= _mod->SPIsetRegValue(SX127X_REG_OCP, SX127X_OCP_ON | raw, 5, 0);
  } else if(currentLimit <= 240) {
    raw = (currentLimit + 30) / 10;
    state |= _mod->SPIsetRegValue(SX127X_REG_OCP, SX127X_OCP_ON | raw, 5, 0);
  }
  return(state);
}

int16_t SX127x::setPreambleLength(uint16_t preambleLength) {
  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // check active modem
  uint8_t modem = getActiveModem();
  if(modem == SX127X_LORA) {
    // check allowed range
    if(preambleLength < 6) {
      return(ERR_INVALID_PREAMBLE_LENGTH);
    }

    // set preamble length
    state = _mod->SPIsetRegValue(SX127X_REG_PREAMBLE_MSB, (uint8_t)((preambleLength >> 8) & 0xFF));
    state |= _mod->SPIsetRegValue(SX127X_REG_PREAMBLE_LSB, (uint8_t)(preambleLength & 0xFF));
    return(state);

  } else if(modem == SX127X_FSK_OOK) {RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

  return(ERR_UNKNOWN);
}

float SX127x::getFrequencyError(bool autoCorrect) {
  int16_t modem = getActiveModem();
  if(modem == SX127X_LORA) {
    // get raw frequency error
    uint32_t raw = (uint32_t)_mod->SPIgetRegValue(SX127X_REG_FEI_MSB, 3, 0) << 16;
    raw |= (uint16_t)_mod->SPIgetRegValue(SX127X_REG_FEI_MID) << 8;
    raw |= _mod->SPIgetRegValue(SX127X_REG_FEI_LSB);

    uint32_t base = (uint32_t)2 << 23;
    float error;

    // check the first bit
    if(raw & 0x80000) {
      // frequency error is negative
      raw |= (uint32_t)0xFFF00000;
      raw = ~raw + 1;
      error = (((float)raw * (float)base)/32000000.0) * (_bw/500.0) * -1.0;
    } else {
      error = (((float)raw * (float)base)/32000000.0) * (_bw/500.0);
    }

    if(autoCorrect) {
      // adjust LoRa modem data rate
      float ppmOffset = 0.95 * (error/32.0);
      _mod->SPIwriteRegister(0x27, (uint8_t)ppmOffset);
    }

    return(error);

  } else if(modem == SX127X_FSK_OOK) {RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  return(-1);
  }

  return(ERR_UNKNOWN);
}

float SX127x::getSNR() {
  // check active modem
  if(getActiveModem() != SX127X_LORA) {
    return(0);
  }

  // get SNR value
  int8_t rawSNR = (int8_t)_mod->SPIgetRegValue(SX127X_REG_PKT_SNR_VALUE);
  return(rawSNR / 4.0);
}

float SX127x::getDataRate() const {
  return(_dataRate);
}

int16_t SX127x::setFrequencyRaw(float newFreq) {
  // set mode to standby
  int16_t state = setMode(SX127X_STANDBY);

  // calculate register values
  uint32_t FRF = (newFreq * (uint32_t(1) << SX127X_DIV_EXPONENT)) / SX127X_CRYSTAL_FREQ;

  // write registers
  state |= _mod->SPIsetRegValue(SX127X_REG_FRF_MSB, (FRF & 0xFF0000) >> 16);
  state |= _mod->SPIsetRegValue(SX127X_REG_FRF_MID, (FRF & 0x00FF00) >> 8);
  state |= _mod->SPIsetRegValue(SX127X_REG_FRF_LSB, FRF & 0x0000FF);
  return(state);
}

size_t SX127x::getPacketLength(bool update) {
  int16_t modem = getActiveModem();

  if(modem == SX127X_LORA) {
    if(_sf != 6) {
      // get packet length for SF7 - SF12
      return(_mod->SPIreadRegister(SX127X_REG_RX_NB_BYTES));

    } else {
      // return the maximum value for SF6
      return(SX127X_MAX_PACKET_LENGTH);
    }

  } else if(modem == SX127X_FSK_OOK) {RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

  return(_packetLength);
}

uint16_t SX127x::getIRQFlags() {
  // check active modem
  if(getActiveModem() == SX127X_LORA) {
    // LoRa, just 8-bit value
    return((uint16_t)_mod->SPIreadRegister(SX127X_REG_IRQ_FLAGS));

  } else {RADIOLIB_DEBUG_PRINTLN(F("FSK NOT IMPLEMENTED!"));
  }

}

uint8_t SX127x::getModemStatus() {
  // check active modem
  if(getActiveModem() != SX127X_LORA) {
    return(0x00);
  }

  // read the register
  return(_mod->SPIreadRegister(SX127X_REG_MODEM_STAT));
}

void SX127x::setRfSwitchPins(RADIOLIB_PIN_TYPE rxEn, RADIOLIB_PIN_TYPE txEn) {
  _mod->setRfSwitchPins(rxEn, txEn);
}

int16_t SX127x::config() {
  // turn off frequency hopping
  int16_t state = _mod->SPIsetRegValue(SX127X_REG_HOP_PERIOD, SX127X_HOP_PERIOD_OFF);
  return(state);
}

bool SX127x::findChip(uint8_t ver) {
  uint8_t i = 0;
  bool flagFound = false;
  while((i < 10) && !flagFound) {
    // reset the module
    reset();

    // check version register
    uint8_t version = _mod->SPIreadRegister(SX127X_REG_VERSION);
    if(version == ver) {
      flagFound = true;
    } else {
      #ifdef RADIOLIB_DEBUG
        RADIOLIB_DEBUG_PRINT(F("SX127x not found! ("));
        RADIOLIB_DEBUG_PRINT(i + 1);
        RADIOLIB_DEBUG_PRINT(F(" of 10 tries) SX127X_REG_VERSION == "));

        char buffHex[12];
        sprintf(buffHex, "0x%02X", version);
        RADIOLIB_DEBUG_PRINT(buffHex);
        RADIOLIB_DEBUG_PRINT(F(", expected 0x00"));
        RADIOLIB_DEBUG_PRINTLN(ver, HEX);
      #endif
      Module::delay(10);
      i++;
    }
  }

  return(flagFound);
}

int16_t SX127x::setMode(uint8_t mode) {
  return(_mod->SPIsetRegValue(SX127X_REG_OP_MODE, mode, 2, 0, 5));
}

int16_t SX127x::getActiveModem() {
  return(_mod->SPIgetRegValue(SX127X_REG_OP_MODE, 7, 7));
}

int16_t SX127x::setActiveModem(uint8_t modem) {
  // set mode to SLEEP
  int16_t state = setMode(SX127X_SLEEP);

  // set modem
  state |= _mod->SPIsetRegValue(SX127X_REG_OP_MODE, modem, 7, 7, 5);

  // set mode to STANDBY
  state |= setMode(SX127X_STANDBY);
  return(state);
}

void SX127x::clearIRQFlags() {
  int16_t modem = getActiveModem();
  if(modem == SX127X_LORA) {
    _mod->SPIwriteRegister(SX127X_REG_IRQ_FLAGS, 0b11111111);
  } 
}

void SX127x::clearFIFO(size_t count) {
  while(count) {
    _mod->SPIreadRegister(SX127X_REG_FIFO);
    count--;
  }
}

#endif
