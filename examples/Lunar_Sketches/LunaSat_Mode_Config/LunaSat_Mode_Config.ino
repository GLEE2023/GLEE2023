#define MODE_1_CONFIG 15043
#define MODE_2_CONFIG 10952

#define MODE_1_DURATION 100
#define MODE_2_DURATION 200

void setup(){
  //setup
}

void loop(){
  if millis()<1000{
    run(MODE1);
    transmitData();
  }else if(){
    run(MODE2);
    transmitData();
  }

}

void setConfig(int config){
  //do maksing to get config for each sensor, currently trash!!!!
  uint_16 acc_config  = (0x3FF<<10) & config;
  uint_16 temp_config = (0x3FF<<20) & config;
  uint_16 mag_config  = (0x3FF<<30) & config;
  uint_16 tp_config   = (0x3FF<<40) & config;

  //accelerometer setup
  if (0x01<<0 & acc_config) {
    /*
    Example:
    acc-config would be a 10 bit bit string looking like 0110110011.
    The "1" in the right most spot This could represent if the accelerometer
      was in accelerometer only or low power wakeup.
    This if statement checks the state of the LSB in the acc_config
      varaible and would set it accordingly by writing to a register
    */
    // Example of writing to accelerometer register
    writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV, 0);
  }
  if (0x01<<1 & acc_config) {

  }
  //As many ifs as necessary

  //....
}


void run(int config){
  setConfig(config);
  while(stoptime<millis()){
    // collect data
  }
}
