#if !defined(_RADIOLIB_SX1272_H)
#define _RADIOLIB_SX1272_H

#include "../../TypeDef.h"

#if !defined(RADIOLIB_EXCLUDE_SX127X)

#include "../../Module.h"
#include "SX127x.h"

// SX1272 specific register map
#define SX1272_REG_AGC_REF                            0x43
#define SX1272_REG_AGC_THRESH_1                       0x44
#define SX1272_REG_AGC_THRESH_2                       0x45
#define SX1272_REG_AGC_THRESH_3                       0x46
#define SX1272_REG_PLL_HOP                            0x4B
#define SX1272_REG_TCXO                               0x58
#define SX1272_REG_PA_DAC                             0x5A
#define SX1272_REG_PLL                                0x5C
#define SX1272_REG_PLL_LOW_PN                         0x5E
#define SX1272_REG_FORMER_TEMP                        0x6C
#define SX1272_REG_BIT_RATE_FRAC                      0x70

// SX1272 LoRa modem settings
// SX1272_REG_FRF_MSB + REG_FRF_MID + REG_FRF_LSB
#define SX1272_FRF_MSB                                0xE4        //  7     0     carrier frequency setting: f_RF = (F(XOSC) * FRF)/2^19
#define SX1272_FRF_MID                                0xC0        //  7     0         where F(XOSC) = 32 MHz
#define SX1272_FRF_LSB                                0x00        //  7     0               FRF = 3 byte value of FRF registers

// SX1272_REG_MODEM_CONFIG_1
#define SX1272_BW_125_00_KHZ                          0b00000000  //  7     6     bandwidth:  125 kHz
#define SX1272_BW_250_00_KHZ                          0b01000000  //  7     6                 250 kHz
#define SX1272_BW_500_00_KHZ                          0b10000000  //  7     6                 500 kHz
#define SX1272_CR_4_5                                 0b00001000  //  5     3     error coding rate:  4/5
#define SX1272_CR_4_6                                 0b00010000  //  5     3                         4/6
#define SX1272_CR_4_7                                 0b00011000  //  5     3                         4/7
#define SX1272_CR_4_8                                 0b00100000  //  5     3                         4/8
#define SX1272_HEADER_EXPL_MODE                       0b00000000  //  2     2     explicit header mode
#define SX1272_HEADER_IMPL_MODE                       0b00000100  //  2     2     implicit header mode
#define SX1272_RX_CRC_MODE_OFF                        0b00000000  //  1     1     CRC disabled
#define SX1272_RX_CRC_MODE_ON                         0b00000010  //  1     1     CRC enabled
#define SX1272_LOW_DATA_RATE_OPT_OFF                  0b00000000  //  0     0     low data rate optimization disabled
#define SX1272_LOW_DATA_RATE_OPT_ON                   0b00000001  //  0     0     low data rate optimization enabled, mandatory for SF 11 and 12 with BW 125 kHz

// SX1272_REG_MODEM_CONFIG_2
#define SX1272_AGC_AUTO_OFF                           0b00000000  //  2     2     LNA gain set by REG_LNA
#define SX1272_AGC_AUTO_ON                            0b00000100  //  2     2     LNA gain set by internal AGC loop

// SX127X_REG_VERSION
#define SX1272_CHIP_VERSION                           0x22

/*!
  \class SX1272

  \brief Derived class for %SX1272 modules. Also used as base class for SX1273.
  Both modules use the same basic hardware and only differ in parameter ranges.
*/
class SX1272: public SX127x {
  public:

    // constructor

    /*!
      \brief Default constructor. Called from Arduino sketch when creating new LoRa instance.

      \param mod Instance of Module that will be used to communicate with the %LoRa chip.
    */
    SX1272(Module* mod);

    // basic methods

    /*!
      \brief %LoRa modem initialization method. Must be called at least once from Arduino sketch to initialize the module.

      \param freq Carrier frequency in MHz. Allowed values range from 860.0 MHz to 1020.0 MHz.

      \param bw %LoRa link bandwidth in kHz. Allowed values are 125, 250 and 500 kHz.

      \param sf %LoRa link spreading factor. Allowed values range from 6 to 12.

      \param cr %LoRa link coding rate denominator. Allowed values range from 5 to 8.

      \param syncWord %LoRa sync word. Can be used to distinguish different networks. Note that value 0x34 is reserved for LoRaWAN networks.

      \param currentLimit Trim value for OCP (over current protection) in mA. Can be set to multiplies of 5 in range 45 to 120 mA and to multiples of 10 in range 120 to 240 mA.
      Set to 0 to disable OCP (not recommended).

      \param preambleLength Length of %LoRa transmission preamble in symbols. The actual preamble length is 4.25 symbols longer than the set number.
      Allowed values range from 6 to 65535.

      \param gain Gain of receiver LNA (low-noise amplifier). Can be set to any integer in range 1 to 6 where 1 is the highest gain.
      Set to 0 to enable automatic gain control (recommended).

      \returns \ref status_codes
    */
    int16_t begin(float freq = 915.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = SX127X_SYNC_WORD, int8_t power = 10, uint16_t preambleLength = 8, uint8_t gain = 0);

    /*!
      \brief Reset method. Will reset the chip to the default state using RST pin.
    */
    void reset() override;

    // configuration methods

    /*!
      \brief Sets carrier frequency. Allowed values range from 860.0 MHz to 1020.0 MHz.

      \param freq Carrier frequency to be set in MHz.

      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

    /*!
      \brief Sets %LoRa link bandwidth. Allowed values are 125, 250 and 500 kHz. Only available in %LoRa mode.

      \param bw %LoRa link bandwidth to be set in kHz.

      \returns \ref status_codes
    */
    int16_t setBandwidth(float bw);

    /*!
      \brief Sets %LoRa link spreading factor. Allowed values range from 6 to 12. Only available in %LoRa mode.

      \param sf %LoRa link spreading factor to be set.

      \returns \ref status_codes
    */
    int16_t setSpreadingFactor(uint8_t sf);
      
    /*!
      \brief Sets %LoRa link coding rate denominator. Allowed values range from 5 to 8. Only available in %LoRa mode.

      \param cr %LoRa link coding rate denominator to be set.

      \returns \ref status_codes
    */
    int16_t setCodingRate(uint8_t cr);

    /*!
      \brief Sets transmission output power. Allowed values range from 2 to 17 dBm.

      \param power Transmission output power in dBm.

      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t power);

    /*!
      \brief Sets gain of receiver LNA (low-noise amplifier). Can be set to any integer in range 1 to 6 where 1 is the highest gain.
      Set to 0 to enable automatic gain control (recommended). Only available in %LoRa mode.

      \param gain Gain of receiver LNA (low-noise amplifier) to be set.

      \returns \ref status_codes
    */
    int16_t setGain(uint8_t gain);



#ifndef RADIOLIB_GODMODE
  protected:
#endif
    int16_t setBandwidthRaw(uint8_t newBandwidth);
    int16_t setSpreadingFactorRaw(uint8_t newSpreadingFactor);
    int16_t setCodingRateRaw(uint8_t newCodingRate);
#ifndef RADIOLIB_GODMODE
  private:
#endif
    bool _ldroAuto = true;
    bool _ldroEnabled = false;

};

#endif

#endif
