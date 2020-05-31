#include "Si702X.hpp"

Si702X::Si702X(TwoWire & i2cbus): i2cbus(i2cbus){
    
}

const byte Si702X::reset(){
    // Send reset command to sensor
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::Reset);

    //byte, which indicates the status of the transmission:
    // 0:success
    // 1:data too long to fit in transmit buffer
    // 2:received NACK on transmit of address
    // 3:received NACK on transmit of data
    // 4:other error
    // https://www.arduino.cc/en/Reference/WireEndTransmission
    return i2cbus.endTransmission();

    // Result is:
    // User register 1 = 0011_1010
    // Heater control register =  0000_0000
}


const byte Si702X::disableHeater(){
    // Read the user config 
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadUser);
    byte info = i2cbus.endTransmission();
    if(info){
        return info;
    }

    i2cbus.requestFrom(address, 1);

    // Clear the heater enable bit
    byte writeData = i2cbus.read() & 0b11111011;

    // Write the user config with the heater enable bit set
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteUser);
    i2cbus.write(writeData);
    
    //byte, which indicates the status of the transmission:
    // 0:success
    // 1:data too long to fit in transmit buffer
    // 2:received NACK on transmit of address
    // 3:received NACK on transmit of data
    // 4:other error
    // https://www.arduino.cc/en/Reference/WireEndTransmission
    return i2cbus.endTransmission();
    
}


const byte Si702X::enableHeater(){
    // Read the user config 
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadUser);
    byte info = i2cbus.endTransmission();
    if(info){
        return info;
    }
    i2cbus.requestFrom(address, 1);
    byte readData = i2cbus.read();

    // Stop if voltage is too low
    if( readData & 0b01000000){
        return 6;
    }

    // Set the heater enable bit
    byte writeData = readData | 0b00000100;

    // Write the user config with the heater enable bit set
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteUser);
    i2cbus.write(writeData);

    //byte, which indicates the status of the transmission:
    // 0:success
    // 1:data too long to fit in transmit buffer
    // 2:received NACK on transmit of address
    // 3:received NACK on transmit of data
    // 4:other error
    // 6:voltage too low
    // https://www.arduino.cc/en/Reference/WireEndTransmission
    return i2cbus.endTransmission();
}


const byte Si702X::setHeater(const byte & level){
    // Stop if level is too high
    if(level > 0b11110000){
        return 5;
    }

    // The datasheet recommends not changing the bits not related to the heater for future compatibility
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadHeatCtrl);
    byte info = i2cbus.endTransmission();
    if(info){
        return info;
    }
    
    // Keep the non-Heater bits and change the heater bits to the given level.
    i2cbus.requestFrom(address, 1);
    byte dataToWrite = i2cbus.read() & 0b11110000;   
    dataToWrite |= level;

    // Write the data with level to the sensor.
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteHeatCtrl);
    i2cbus.write(dataToWrite);

    //byte, which indicates the status of the transmission:
    // 0:success
    // 1:data too long to fit in transmit buffer
    // 2:received NACK on transmit of address
    // 3:received NACK on transmit of data
    // 4:other error
    // 5:level too high
    // https://www.arduino.cc/en/Reference/WireEndTransmission
    return i2cbus.endTransmission();
}

const byte Si702X::setResolution(const bool & a, const bool & b){
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadHeatCtrl);
    byte info = i2cbus.endTransmission();
    if(info){
        return info;
    }
    
    // Keep the non-Resolution bits and change the Resolution bits to the given precision.
    i2cbus.requestFrom(address, 1);
    byte dataToWrite = i2cbus.read() & 0b01111110;
    dataToWrite |= a;
    dataToWrite |= b << 8;

    // Write the data with level to the sensor.
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteHeatCtrl);
    i2cbus.write(dataToWrite);

    //byte, which indicates the status of the transmission:
    // 0:success
    // 1:data too long to fit in transmit buffer
    // 2:received NACK on transmit of address
    // 3:received NACK on transmit of data
    // 4:other error
    // https://www.arduino.cc/en/Reference/WireEndTransmission
    return i2cbus.endTransmission();
}


const float Si702X::getRH() const{
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::HumidityHoldMaster);
    i2cbus.endTransmission();
    i2cbus.requestFrom(address, 2);


    int16_t humidity  = (i2cbus.read() << 8) + i2cbus.read();
    return ((125.0 * humidity) / 65536.0) - 6;
}


const float Si702X::getCelcius() const{
    // Read data from sensor
    // Might change address written
    // Havent found reason for hold master and not hold master
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::TemperatureHoldMaster);
    i2cbus.endTransmission();
    i2cbus.requestFrom(address, 2);

    // Convert data to Celcius
    int16_t temp  = (i2cbus.read() << 8) + i2cbus.read();
    return ((175.72 * temp) / 65536.0) - 46.85;
}


const float Si702X::getKelvin() const{
    // An absolute scale of temperature. 
    // The temperature difference of one Kelvin degree is defined as equal to one Celcius degree.

    // Converts Celcius to Kelvin
    return getCelcius() + 273.15;
}


const float Si702X::getFahrenheit() const{
    // Converts Celcius to Fahrenheit
    return getCelcius() * 9.0 / 5.0 + 32;
}


const float Si702X::getNewton() const{
    // The Newton scale is a temperature scale that sets a fixed zero point equal to the freezing point water. 
    // The scale uses both arithmetic progression and geometric progression. 
    // The scale defines 18 reference points and the corresponding "degrees of heat" and geometric progressions.

    // Converts Celcius to Newton
    return getCelcius() * 0.33;
}


const float Si702X::getRankine() const{
    // An absolute scale of temperature. 
    // The temperature difference of one Rankine degree is defined as equal to one Fahrenheit degree.
    // Fahrenheit Equivalent for Kelvin to Celcius.

    // Converts Celcius to Rankine
    return ( getCelcius() + 273.15 ) * 9.0 / 5.0;
}


const float Si702X::getDelisle() const{
    // The Delisle scale is a scale to measure temperature. It was introduced by French astronomer Joseph-Nicolas Delisle in 1732. 
    // 0 Delisle is equal to the boiling point of water under normal pressure. 
    // Josias Weitbrecht recalibrated the theromometer in 1738, so that the melting point of ice was taken into account. Ice melts at 150°De.
    // The Deslile scale counts downwards, from the boiling point to the melting point. It was widely used in Russia in the 19th century.

    // Converts Celcius to Delisle
    return (100 - getCelcius()) * 3.0 / 2.0;
}


const float Si702X::getReaumur() const{
    //The Réaumur scale is a temperature scale for which the freezing and boiling points of water are defined as 0 and 80 degrees. 
    // The scale is named for René Antoine Ferchault de Réaumur, who first proposed a similar scale in 1730.

    // Converts Celcius to Reaumur
    return getCelcius() * 0.8;
}



const float Si702X::getRomer() const{
    // In this scale, the zero was initially set using freezing brine. 
    // The boiling point of water was defined as 60 degrees. 
    // The lower fixed point is the freezing point of water at precisely 7.5 degrees. 

    // The inventor of the Fahrenheit scale Daniel Gabriel Fahrenheit learned of Rømer's work increasing 
    // the number of divisions by a factor of four and establishing what is now known as the Fahrenheit scale.

    // Converts Celcius to Romer
    return getCelcius() * 21.0 / 40.0 + 7.5;
}


const int16_t Si702X::getSerial() const{
    // Request serial from sensor
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::SerialFirst);
    i2cbus.write(regAddress::SerialSecond);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 2);

    // Add 2 bytes together to get the full serial
    return (i2cbus.read() << 8) + i2cbus.read();
}


const String Si702X::getDeviceID()const{
    // Request device id from sensor
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::DeviceIdFirst);
    i2cbus.write(regAddress::DeviceIdSecond);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 1);

    // Find corrosponding code and return the device ID
    switch(i2cbus.read()){
        case 0x00:
        case 0xFF:
            return "Engineering sample";

        case 0x0D:
            return "Si7013";

        case 0x14:
            return "Si7020";

        case 0x15:
            return "Si7021";

        default:
            return "Unknown";
    }
}


const String Si702X::getVersion() const{
    // Request firmware version from sensor
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::FirmwareFirst);
    i2cbus.write(regAddress::FirmwareSecond);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 1);

    // Find corrosponding code and return the firmware version
    switch(i2cbus.read()){
        case 0x20:
            return "Firmware version 2.0";

        case 0xFF:
            return "Firmware version 1.0";

        default:
            return "Firmware version unknown";
    }
}