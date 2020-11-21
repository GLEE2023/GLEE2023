#ifndef _RADIOLIB_PHYSICAL_LAYER_H
#define _RADIOLIB_PHYSICAL_LAYER_H

#include "../../TypeDef.h"

/*!
  \class PhysicalLayer

  \brief Provides common interface for protocols that run on %LoRa/FSK modules, such as RTTY or LoRaWAN. Also extracts some common
  module-independent methods. Using this interface class allows to use the protocols on various modules without much code duplicity.
  Because this class is used mainly as interface, all of its virtual members must be implemented in the module class.
*/
class PhysicalLayer {
  public:

    // constructor

    /*!
      \brief Default constructor.

      \param freqStep Frequency step of the synthesizer in Hz.

      \param maxPacketLength Maximum length of packet that can be received by the module-
    */
    PhysicalLayer(float freqStep, size_t maxPacketLength);

    // basic methods

    /*!
      \brief Arduino Flash String transmit method.

      \param str Pointer to Arduino Flash String that will be transmitted.

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    int16_t transmit(__FlashStringHelper* fstr, uint8_t addr = 0);

    /*!
      \brief Arduino String transmit method.

      \param str Address of Arduino string that will be transmitted.

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    int16_t transmit(String& str, uint8_t addr = 0);

    /*!
      \brief C-string transmit method.

      \param str C-string that will be transmitted.

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    int16_t transmit(const char* str, uint8_t addr = 0);

    /*!
      \brief Binary transmit method. Must be implemented in module class.

      \param data Binary data that will be transmitted.

      \param len Length of binary data to transmit (in bytes).

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    virtual int16_t transmit(uint8_t* data, size_t len, uint8_t addr = 0) = 0;

    /*!
      \brief Arduino String receive method.

      \param str Address of Arduino String to save the received data.

      \param len Expected number of characters in the message. Leave as 0 if expecting a unknown size packet

      \returns \ref status_codes
    */
    int16_t receive(String& str, size_t len = 0);

    /*!
      \brief Sets module to standby.

      \returns \ref status_codes
    */
    virtual int16_t standby() = 0;

    /*!
      \brief Binary receive method. Must be implemented in module class.

      \param data Pointer to array to save the received binary data.

      \param len Number of bytes that will be received. Must be known in advance for binary transmissions.

      \returns \ref status_codes
    */
    virtual int16_t receive(uint8_t* data, size_t len) = 0;

    /*!
      \brief Interrupt-driven Arduino String transmit method. Unlike the standard transmit method, this one is non-blocking.
      Interrupt pin will be activated when transmission finishes.

      \param str Address of Arduino String that will be transmitted.

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    int16_t startTransmit(String& str, uint8_t addr = 0);

    /*!
      \brief Interrupt-driven Arduino String transmit method. Unlike the standard transmit method, this one is non-blocking.
      Interrupt pin will be activated when transmission finishes.

      \param str C-string that will be transmitted.

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    int16_t startTransmit(const char* str, uint8_t addr = 0);

    /*!
      \brief Interrupt-driven binary transmit method.

      \param data Binary data that will be transmitted.

      \param len Length of binary data to transmit (in bytes).

      \param addr Node address to transmit the packet to. Only used in FSK mode.

      \returns \ref status_codes
    */
    virtual int16_t startTransmit(uint8_t* data, size_t len, uint8_t addr = 0) = 0;

    /*!
      \brief Reads data that was received after calling startReceive method.

      \param str Address of Arduino String to save the received data.

      \param len Expected number of characters in the message.

      \returns \ref status_codes
    */
    int16_t readData(String& str, size_t len = 0);

    /*!
      \brief Reads data that was received after calling startReceive method.

      \param data Pointer to array to save the received binary data.

      \param len Number of bytes that will be received. Must be known in advance for binary transmissions.

      \returns \ref status_codes
    */
    virtual int16_t readData(uint8_t* data, size_t len) = 0;


    /*!
      \brief Gets the module frequency step size that was set in constructor.

      \returns Synthesizer frequency step size in Hz.
    */
    float getFreqStep() const;

    /*!
     \brief Query modem for the packet length of received payload. Must be implemented in module class.

     \param update Update received packet length. Will return cached value when set to false.

     \returns Length of last received packet in bytes.
   */
   virtual size_t getPacketLength(bool update = true) = 0;


#ifndef RADIOLIB_GODMODE
  private:
#endif
    float _freqStep;
    size_t _maxPacketLength;
};

#endif
