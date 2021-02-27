# Coding Style Guidelines 
This document is intended for everyone who contributes to the GLEE2023 code. The basic convnetions for naming, indentation, and comments are explained here. These guidelines will uniformize the overall look of the code and will aim to increase organization, readability and as a result, faster comprehension of the code. 

## Naming Convention
* We mostly follow the standard C++ naming conventions
* Variables: nouns, camel case with the first word in small case and the first letter of each consecutive word capitalized
```cpp
float currentFactor = 16384.0;
```
* Registers: nouns, all capitalized with an underscore between each word
```cpp
#define AK09940_WAI_1 0x00
```
* Functions: verbs, camel case with the first word in small case and the first letter of each consecutive word capitalized
```cpp
LunaRadio::setBandwidth(bw);
```
* Classes: nouns, first letter of each word capitalized
```cpp
class LunaSat{
```
* Folders: nouns, first letter of each word capitalized with an underscore between each word e.g. GLEE_TMP117

## Indentation and Brackets
* One tab = four spaces
```cpp
AK09940::AK09940(int _id, bool _debug){
    AK09940::info.id = _id;
```
* Nested blocks of code should be properly indented with each new block adding an additional tab
```cpp
int32_t AK09940::interpret18BitAs32Bit(int32_t input){
    // The output of the magnetometer is an 18 bit output, for ease of use it is converted to a 32bit output)
    if(bitRead(input, 17) == 0){
	    // If the MSB of the 18 bit output is 0 - this means output is positive

	    // Set all bits from [32:18] to 0 so that the output is positive and unchanged by the extra leading bits
		for(int i = 31; i >= 17; i--){								
		    bitWrite(input, i, 0);
	    }
    }
```
* Open brackets should be on the same line as what precedes it; they should always be immediately after the close paranthesis
```cpp
Capacitive::Capacitive(int _id, bool _debug){
    Capacitive::info.id = _id;
```

## Comments
* Block comments: There should be a block comment immediately before each function that lists the parameters, what this function returns, and the purpose of this function.
```cpp
/*
Parameters: spreading factor as an 8 bit unsigned integer
Returns: none
This function sets the spreading factor and if successful, it will construct a
display message and send that message to serial to notify the user. If
the frequency failed, it will output a message.
*/
void LunaRadio::setSF(uint8_t sf){
```
* One-line comments: When needed, a line of comment should be used to explain the function and purpose of this line of code when it is more complex and not self-explanatory. This type of comment should be in its own line immediately preceding the line of code it is describing. If more detailed explanataion is needed, it can be added as an in-line comment in the same line as the line of code described.
```cpp
// Data processing variables
uint16_t readByte;			// byte to store data that is read
uint8_t data[2];			// Declares an array of length 2 to be empty
```