# Si702X sensor
Arduino library for Si7020 and Si7021 sensors.  

Used to measure temperature in various scales, measure the relative humidity and implements a heater for frost removal.


## Usage
This is an example how you could use this library.
```cpp
#include <Arduino.h>
#include "Si702X.hpp"

//Create sensor object.
Si702X sensor = Si702X();
void setup() {

    //The Si702X does not start the i2c bus on its own.
    Wire.begin();

    //Start the serial connection for printing the readings
    Serial.begin(9600);

    //Print the sensors firmware version
    Serial.print("The sensor has ");
    Serial.println( sensor.getVersion() );
   
    //Print the sensors device ID
    Serial.print("The device ID is ");
    Serial.println( sensor.getDeviceID() );

    //Print the sensors serial number
    Serial.print("The Serial number is ");
    Serial.print( sensor.getSerial() );
    Serial.println("\n");
}

void loop() {
    //Wait for half a second
    delay(500);

    //Print the temperature reading in Fahrenheit
    Serial.print("The temperature is ");
    Serial.print( sensor.getFahrenheit() );
    Serial.println(" Fahrenheit.");

    //Print the temperature reading in celcius
    Serial.print("The temperature is ");
    Serial.print( sensor.getCelcius() );
    Serial.println(" Celcius.");

    //Print the relative humidity
    Serial.print("The relative humidity is ");
    Serial.print( sensor.getRH() );
    Serial.println("\n");
}
```

## Documentation
Here are the functions explained that can be used with this library

### Constructor
The constructor has the option to give a TwoWire object to send commands through. The default is the standard Arduino Wire object and does not have to be given to the constructor if used. The Si702X assumes the .begin() function has been used on the TwoWire bus.

### Temperature functions
All these functions return the temperature in celcius and converts them to the temperature scale in their name. The resolution is determined by the [setResolution](#-setResolution(a,-b)) function. These functions can be influenced by the heater element if used. These functions are possible to use whilst the sensor is const.

**List of temperature functions with the part after "get" being the scale it returns.**
- getCelcius()
- getKelvin()
- getFahrenheit()
- getNewton()
- getRankine()
- getDelisle()
- getReaumur()
- getRomer()

### Relative Humidity function
The function ``` getRH() ``` gets the relative humidity that the sensor has measured. The reading is returned in a const float. The resolution is determined by the [setResolution](#-setResolution(a,-b)) function. So far I havent found the need or the wil to implement more scales to view the humidity. These functions can be influenced by the heater element if used.

### Control functions
Functions in this catagory are configure the sensors heater and reading resolution.
These functions are not possible to use if the sensor is const. These functions make use of the i2c error codes with some of my own added. The error code have the following meaning:
| Error code | Meaning | 
| ---------- | ------- |
| 0 | Succesfully executed function |
| 1 | Data too long to fit in transmit buffer |
| 2 | Received NACK on transmit of address |
| 3 | Received NACK on transmit of data |
| 4 | Other error |
| 5 | Given heater level too high
| 6 | Voltage too low |

####    reset()
This function resets all the settings used for this sensor.

####    enableHeater()
This function enables the heater and lets it heat up to the given [level](#-setHeater()). If the given voltage to the sensor is not high enough the heater wil not enable.

####    disableHeater();
This function disables the heater but remembers the level that has been set.

####    setHeater(level)
Sets the heat of the sensor.  
The heater can be set to a level between 0 - 15. The higher number the higher the heat.  
A level above 15 will return 5 and not change the heater level.  
Heater needs to be [enabled](#-enableHeater()) to take effect.  
If no argument is given the level will be 0 i.e. off  

The heater level has the following typical current draw in mA.  
Assumes VDD = 3.3 V.  
| Heater level (binary) | Current draw (mA)  |
| ------------ | ------------------ |
| 0000 | 3.09 |
| 0001 | 9.18 |
| 0010 | 15.24 |
| ...  | ... |
| 0100 | 27.39 |
| ...  | ... | 
| 1000 | 51.69 |
| ...  | ... |
| 1111 | 94.40


####    setResolution(a, b)
Sets the resolution of temperature and humidity measurements.
Measurement Resolution can be configured using the following table:
| Config    | RH precision  | Temp precision|
| --------- | ------------  | ------------- |
| 00        | 12 bit        |  14 bit       |
| 01        | 8 bit         |  12 bit       |
| 10        | 10 bit        |  13 bit       |
| 11        | 11 bit        |  11 bit       |

### Other functions

####    getVersion()
Returns the version of the sensor in a string

####    getDeviceID()
Returns the device ID of the sensor in a string

####    getSerial()
Returns the serial number of the sensor in a 16bit signed int.


## Used information
- [Datasheet](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf).
