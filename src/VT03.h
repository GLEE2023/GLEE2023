/**
 * Description: This is an Arduino (C++) Header file required for the VT03 RADFET
 * Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
 * Library Contributors: Yvonne Liu
 * Primary Resources Used in Creation:
 * VT03 Datasheet (https://www.varadis.com/wp-content/uploads/2021/07/VT03-Datasheet_rev2p0.pdf)
 *
**/

#ifndef VT03_H
#define VT03_H

#include <Arduino.h>
#include "GLEE_Sensor.h"

//RADFET == sensitive to ionizing radiona (gamma, X-rays, protons)
//GY == gray (unit of ionizing radiation does in SI units, 
// absorption of one joule of radiation energy per kilogram of matter)
// read-out in 5-9 volts
// Irradiation: the process by which an object is exposed to radiation
// Dose rate: quantity of radiation absorbed or delivered per unit time
// 2 modes- irradiation mode (measures radiation, all terminals should be grounded and read-out mode (current is applied to the RADFET in two-terminal mode and voltage
// reading is taken.)
//1.varadis meeting 2. reach out to lunasat and sceince 3. cap sensor- pins (turn pin on then read out)

class VT03:public Sensor{

    public:
        VT03(int _id, bool _debug = false);
		void begin();
		uint8_t getAddress();
		int getRawData();

    private:
        int pin;
        
};
#endif