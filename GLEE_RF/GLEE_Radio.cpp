#include "GLEE_Radio.h"

void LunaRadio::initialize_radio(float freq, uint8_t pwr, float bw, uint8_t sf, uint8_t cr){
	
	Serial.print(F("Radio Initializing ... "));
	LunaRadio::err_state = LunaRadio::radio.begin();
	if (LunaRadio::err_state == ERR_NONE) {
		Serial.println(F("success!"));
	} else {
		Serial.print(F("failed, code "));
		Serial.println(LunaRadio::err_state);
		while (true);
	}
    
    //set frequency
    LunaRadio::setFreq(freq);

    //set bandwidth
    LunaRadio::setBandwidth(bw);

    //set output power
    LunaRadio::setPWR(pwr);

    //set spreading factor
    LunaRadio::setSF(sf);

    //set coding rate
    LunaRadio::setCR(cr);

    Serial.println("The radio is ready for use!");
	
}

void LunaRadio::setFreq(float freq){
	LunaRadio::err_state = LunaRadio::radio.setFrequency(freq);
  	if (LunaRadio::err_state == ERR_NONE) {
  		snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Frequency set to %d.%01d MHz, success!",(int)freq,(int)(freq*10)%10);
    	Serial.println(LunaRadio::disp_buff);
  	} else {
    	Serial.print("Frequency set failed, code ");
    	Serial.println(LunaRadio::err_state);
    	while (true);
  	}
}

void LunaRadio::setCR(uint8_t cr){
	LunaRadio::err_state = LunaRadio::radio.setCodingRate(cr);
    if (LunaRadio::err_state == ERR_NONE) {
      snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Coding Rate set to %d, success!",cr);
      Serial.println(LunaRadio::disp_buff);
    } else {
      Serial.print("Coding Rate set failed, code ");
      Serial.println(LunaRadio::err_state);
      while (true);
    }
}

void LunaRadio::setSF(uint8_t sf){
  	LunaRadio::err_state = LunaRadio::radio.setSpreadingFactor(sf);
    if (LunaRadio::err_state == ERR_NONE) {
      snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Spreading factor set to %d, success!",sf);
      Serial.println(LunaRadio::disp_buff);
    } else {
      Serial.print("Spreading Factor set failed, code ");
      Serial.println(LunaRadio::err_state);
      while (true);
    }
}

void LunaRadio::setBandwidth(float bw){
	LunaRadio::err_state = LunaRadio::radio.setBandwidth(bw);
  	if (LunaRadio::err_state == ERR_NONE) {
    	snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Bandwidth set to %d.%01d, success!",(int)bw,(int)(bw*10)%10);
    	Serial.println(LunaRadio::disp_buff);
  	} else {
    	Serial.print("Bandwidth set failed, code ");
    	Serial.println(LunaRadio::err_state);
    	while (true);
  	}
}

void LunaRadio::setPWR(uint8_t pwr){
  	LunaRadio::err_state = LunaRadio::radio.setOutputPower(pwr);
  	if (LunaRadio::err_state == ERR_NONE) {
    	snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Output Power set to %d, success!",pwr);
      	Serial.println(LunaRadio::disp_buff);
  	} else {
    	Serial.print("Output Power set failed, code ");
    	Serial.println(LunaRadio::err_state);
    	while (true);
  	}
}


void LunaRadio::transmit_data(char* buff){
  Serial.println("Transmitting Data");
  LunaRadio::err_state = LunaRadio::radio.transmit(buff);
  if (LunaRadio::err_state == ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("Tranmission success!"));

    // print measured data rate
    Serial.print(F("Datarate: "));
    Serial.print(LunaRadio::radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (LunaRadio::err_state == ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (LunaRadio::err_state == ERR_TX_TIMEOUT) {
    // timeout occurred while transmitting packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(LunaRadio::err_state);

  }
}

String LunaRadio::receive_data_string(void){
	Serial.println("Waiting for Data");
	String tmp_str;
	LunaRadio::err_state = LunaRadio::radio.receive(tmp_str);
	if(LunaRadio::err_state == ERR_NONE){
		Serial.println("Data received");
		return tmp_str;
	}else if(LunaRadio::err_state == ERR_RX_TIMEOUT){
		Serial.println("Receive Failed");
		return "Receiver Timeout";
	}else if(LunaRadio::err_state == ERR_CRC_MISMATCH){
		Serial.println("Receive Failed");
		return "CTC Error";
	}else{
		Serial.print(F("failed, code "));
		Serial.println(LunaRadio::err_state);
		return "Check Code";
	}
}