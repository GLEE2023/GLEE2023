#include "GLEE_Radio.h"

/*
Parameters: frequency as a float, power as an unsigned 8 bit integer, bandwith as a float,
spreading factoctor as an unsigned 8 bit integer, and coding rate as an an unsigned 8 bit integer
Returns: none
This function begins communications with the radio and notifies if the communication
was successful or not. Then, it sets the frequency, bandwith, output power, spreading
factor, and coding rate if successful. 
*/
void LunaRadio::initialize_radio(float freq, uint8_t pwr, float bw, uint8_t sf, uint8_t cr){
	
	Serial.print(F("Radio Initializing ... "));
	LunaRadio::err_state = LunaRadio::radio.begin(); //begin communication with radio
	if (LunaRadio::err_state == ERR_NONE) {
		//if no error
		Serial.println(F("success!"));
	} else {
		//if failed and error
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


/*
Parameters: frequency as a float
Returns: none
This function sets the frequency and if successful, it will construct a
display message and send that message to serial to notify the user. If
the frequency failed, it will output a message.
*/
void LunaRadio::setFreq(float freq){
	LunaRadio::err_state = LunaRadio::radio.setFrequency(freq); //saves the error state and sets frequency
  	if (LunaRadio::err_state == ERR_NONE) {
		  //constructing display message, then sends message to serial if no error 
  		snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Frequency set to %d.%01d MHz, success!",(int)freq,(int)(freq*10)%10);
    	Serial.println(LunaRadio::disp_buff);
  	} else {
		  //if failed and error
    	Serial.print("Frequency set failed, code ");
    	Serial.println(LunaRadio::err_state);
    	while (true);
  	}
}

/*
Parameters: coding rate as an 8 bit unsigned integer
Returns: none
This function sets the coding rate and if successful, it will construct a
display message and send that message to serial to notify the user. If
the frequency failed, it will output a message.
*/
void LunaRadio::setCR(uint8_t cr){
	LunaRadio::err_state = LunaRadio::radio.setCodingRate(cr); //sets coding rate and saves error state
    if (LunaRadio::err_state == ERR_NONE) {
		//constructing display message, then sends message to serial if no error
      snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Coding Rate set to %d, success!",cr);
      Serial.println(LunaRadio::disp_buff);
    } else {
		//if failed and error
      Serial.print("Coding Rate set failed, code ");
      Serial.println(LunaRadio::err_state);
      while (true);
    }
}

/*
Parameters: spreading factor as an 8 bit unsigned integer
Returns: none
This function sets the spreading factor and if successful, it will construct a
display message and send that message to serial to notify the user. If
the frequency failed, it will output a message.
*/
void LunaRadio::setSF(uint8_t sf){
  	LunaRadio::err_state = LunaRadio::radio.setSpreadingFactor(sf); //saves the error state and sets spreading factor
    if (LunaRadio::err_state == ERR_NONE) {
		//constructing display message, then sends message to serial if no error
      snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Spreading factor set to %d, success!",sf);
      Serial.println(LunaRadio::disp_buff);
    } else {
		//if failed and error
      Serial.print("Spreading Factor set failed, code ");
      Serial.println(LunaRadio::err_state);
      while (true);
    }
}

/*
Parameters: bandwith as a float
Returns: none
This function sets the bandwith and if successful, it will construct a
display message and send that message to serial to notify the user. If
the frequency failed, it will output a message.
*/
void LunaRadio::setBandwidth(float bw){
	LunaRadio::err_state = LunaRadio::radio.setBandwidth(bw); //saves the error state and sets bandwith
  	if (LunaRadio::err_state == ERR_NONE) {
		  //constructing display message, then sends message to serial if no error
    	snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Bandwidth set to %d.%01d, success!",(int)bw,(int)(bw*10)%10);
    	Serial.println(LunaRadio::disp_buff);
  	} else {
		  //if failed and error
    	Serial.print("Bandwidth set failed, code ");
    	Serial.println(LunaRadio::err_state);
    	while (true);
  	}
}

/*
Parameters: output power as an 8-bit unsigned integer
Returns: none
This function sets the output power and if successful, it will construct a
display message and send that message to serial to notify the user. If
the frequency failed, it will output a message.
*/
void LunaRadio::setPWR(uint8_t pwr){
  	LunaRadio::err_state = LunaRadio::radio.setOutputPower(pwr); //sets output and saves error state
  	if (LunaRadio::err_state == ERR_NONE) { 
		  //constructing display message, then sends message to serial if no error
    	snprintf(LunaRadio::disp_buff, sizeof(LunaRadio::disp_buff),"Output Power set to %d, success!",pwr); //
      	Serial.println(LunaRadio::disp_buff);
  	} else {
		//if error and failed
    	Serial.print("Output Power set failed, code ");
    	Serial.println(LunaRadio::err_state);
    	while (true);
  	}
}

/*
Parameters: the buffer as a char pointer
Returns: none
This function transmits data from what is given as the argument and prints
out the  easured data rate if successful and the error message if unsuccessful.
*/
void LunaRadio::transmit_data(char* buff){
  Serial.println("Transmitting Data");
  LunaRadio::err_state = LunaRadio::radio.transmit(buff); //saves error state when trasnmitting
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

/*
Parameters: none
Returns: a string that is the data received or a message of the error
This function receives data while checking and outputting error if there is any.
*/
String LunaRadio::receive_data_string(void){
	Serial.println("Waiting for Data");
	String tmp_str;
	LunaRadio::err_state = LunaRadio::radio.receive(tmp_str); //saves error state when receiving
	if(LunaRadio::err_state == ERR_NONE){ 
		//if no eror
		Serial.println("Data received");
		return tmp_str;
	}else if(LunaRadio::err_state == ERR_RX_TIMEOUT){ 
		//timeout error
		Serial.println("Receive Failed");
		return "Receiver Timeout";
	}else if(LunaRadio::err_state == ERR_CRC_MISMATCH){ 
		//CTC error
		Serial.println("Receive Failed");
		return "CTC Error";
	}else{
		Serial.print(F("failed, code ")); 
		//code error
		Serial.println(LunaRadio::err_state);
		return "Check Code";
	}
}

/*
Parameters: none
Returns: a string that or states the error 
This function receives data while checking and outputting error if there is any
 and is used for plotting.
*/
String LunaRadio::receive_data_string_plotting(void){
	String tmp_str;
	LunaRadio::err_state = LunaRadio::radio.receive(tmp_str); //saves error state when receiving
	if(LunaRadio::err_state == ERR_NONE){ 
		//if no eror
		return tmp_str;
	}else if(LunaRadio::err_state == ERR_RX_TIMEOUT){ 
		//timeout error
		Serial.println("Receive Failed");
		return "Receiver Timeout";
	}else if(LunaRadio::err_state == ERR_CRC_MISMATCH){ 
		//CTC error
		Serial.println("Receive Failed");
		return "CTC Error";
	}else{
		Serial.print(F("failed, code ")); 
		//code error
		Serial.println(LunaRadio::err_state);
		return "Check Code";
	}
}

/*
Parameters: pointer to recieve interupt callback function
Returns: none
Sets recieve 
*/
void LunaRadio::enable_recieve_interupt(void (*func)){

	radio.setDio0Action(func);

	int state = radio.startReceive();
  	if (state == ERR_NONE) {
    	Serial.println(F("Now listening for transmissions"));
  	} else {
    	Serial.print(F("failed, code "));
    	Serial.println(state);
    while (true);
  }
}


void LunaRadio::startRecieve(void){
	int state = radio.startReceive();
  	if (state == ERR_NONE) {
    	Serial.println(F("Now listening for transmissions"));
  	} else {
    	Serial.print(F("failed, code "));
    	Serial.println(state);
    while (true);
  }
}

void LunaRadio::readData(uint8_t* data, size_t len){
	int state = radio.readData(data,len);
	if (state == ERR_NONE) {
      // packet was successfully received
      Serial.println(F("Packet received!"));



    } else if (state == ERR_CRC_MISMATCH) {
      // malformed packet reception
      Serial.println(F("Malformed Packet!"));

    } else {
      // Print unhandled error coad
      Serial.print(F("Failed, code "));
      Serial.println(state);
    }
}