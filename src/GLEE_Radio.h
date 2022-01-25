/**
 * Description: This is an Arduino (C++) .h file required for the RF
 * Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
 * Resources Used in Creation:
 * RadioLib (Base functionality) ()
**/

#ifndef _GLEE_Radio_H
#define _GLEE_Radio_H

#include <RadioLib.h>
#include <string.h>
#include <Arduino.h>

#define NSS_PIN 10 
#define DIO1_PIN 3
#define DIO0_PIN 2
#define RESET_PIN 9

class LunaRadio{
	public:
		String cr_buff; //buffer
		int err_state; //error state
	
		//Begins communications with the radio and notifies if the communication was successful or not.
		void initialize_radio(float freq = 915.0, uint8_t pwr = 17, float bw = 250.0, uint8_t sf = 12, uint8_t cr = 8);
	
		//Transmits data from parameters and prints out the neasured data rate
		void transmit_data(char* buff);
	
		
		String receive_data_string(void);
		String receive_data_string_plotting(void);
	
		//Sets frequency, Coding Rate, Spreading Factor, Bandwidth and Power
		void setFreq(float freq);
		void setCR(uint8_t cr);
		void setSF(uint8_t sf);
		void setBandwidth(float bw);
		void setPWR(uint8_t pwr);
		
		//Sets and Starts Recieve
		void enable_recieve_interupt(void (*func));
		void startRecieve(void);
	
		//Determines if data holds error
		void readData(uint8_t* data, size_t len);

		// Signal Metrics
		float getRSSI(void);
		float getSNR(void);

	private:
		SX1272 radio = new Module(NSS_PIN, DIO0_PIN, RESET_PIN, DIO1_PIN); //creates new module (from radiolib)
		char disp_buff[50]; //display buffer for creating messages
};

#endif
