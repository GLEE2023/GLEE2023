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
	String cr_buff;
	int err_state;
	void initialize_radio(float freq = 915.0, uint8_t pwr = 17, float bw = 250.0, uint8_t sf = 12, uint8_t cr = 8);
	void transmit_data(char* buff);
	String receive_data_string(void);
	void setFreq(float freq);
	void setCR(uint8_t cr);
	void setSF(uint8_t sf);
	void setBandwidth(float bw);
	void setPWR(uint8_t pwr);

private:
	SX1272 radio = new Module(NSS_PIN, DIO0_PIN, RESET_PIN, DIO1_PIN);
	float c_freq;
	uint8_t output_pwr;
	float bandwidth;
	uint8_t spread_factor;
	uint8_t coding_rate;
	char disp_buff[50];
};

#endif