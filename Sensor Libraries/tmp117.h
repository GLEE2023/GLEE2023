/*


*/



//Define Senssor Register Address Below

#define TEMP_RESULT 0x00









class TMP117 : public Sensor 
{

    public: 

    float getTemperatureC(void);
    float getTemperatureF(void);
    float getUncertainty(float inputTemperature);


    uint16_t getRawTemperature(void)
    


    private: 



}