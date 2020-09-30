








float TMP117 :: getTemperatureC(void)

{
    uint8_t lowByte, highByte;
    lowByte = TMP117.readByte(Temperature_Output_Byte_Low);
    highByte = TMP117.readByte(Temperature_Output_Byte_High);

    uint16_t rawTempOutput = (highByte<<8 | lowByte);
}