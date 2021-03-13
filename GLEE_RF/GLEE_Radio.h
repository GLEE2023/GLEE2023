#ifndef _GLEE_Radio_H
#define _GLEE_Radio_H

/*
*Description: This is an Arduino (C++) .h file required for the RF
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Resources Used in Creation:
*RadioLib
*Arduino Wire Library Reference Guide
*/

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
	void initialize_radio(float freq = 915.0, uint8_t pwr = 17, float bw = 250.0, uint8_t sf = 12, uint8_t cr = 8);
	void transmit_data(char* buff);
	String receive_data_string(void);
	String receive_data_string_plotting(void);
	void setFreq(float freq);
	void setCR(uint8_t cr);
	void setSF(uint8_t sf);
	void setBandwidth(float bw);
	void setPWR(uint8_t pwr);
	void enable_recieve_interupt(void (*func));
	void startRecieve(void);
	void readData(uint8_t* data, size_t len);

	int getRSSI(void);
	int getSNR(void);

private:
	SX1272 radio = new Module(NSS_PIN, DIO0_PIN, RESET_PIN, DIO1_PIN); //creates new module (from radiolib)
	char disp_buff[50]; //display buffer for creating messages
};

#endif