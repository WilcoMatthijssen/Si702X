#include "Si702X.hpp"

Si702X::Si702X(TwoWire & i2cbus): i2cbus(i2cbus){
    
}


bool Si702X::disableHeater(){
    // Read the user config 
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadUser);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 1);

    // Clear the heater enable bit
    byte writeData = i2cbus.read() & 0b11111011;

    // Write the user config with the heater enable bit set
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteUser);
    i2cbus.write(writeData);
    i2cbus.endTransmission();

    // The heater is disabled succesfully
    return true;
}


bool Si702X::enableHeater(){
    // Read the user config 
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadUser);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 1);
    byte readData = i2cbus.read();

    // Return false if voltage is too low
    if( readData & 0b01000000){
        return false;
    }

    // Set the heater enable bit
    byte writeData = readData | 0b00000100;

    // Write the user config with the heater enable bit set
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteUser);
    i2cbus.write(writeData);
    i2cbus.endTransmission();

    // The heater is enabled succesfully
    return true;
}


bool Si702X::setHeater(const byte & level){
    // If the level exceeds the maximum level return false and not change the level
    if(level > 0b11110000){
        return false;
    }

    // The datasheet recommends not changing the bits not related to the heater for future compatibility
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::ReadHeatCtrl);
    i2cbus.endTransmission(false);
    
    // Keep the non-Heater bits and change the heater bits to the given level.
    i2cbus.requestFrom(address, 1);
    byte dataToWrite = i2cbus.read() & 0b11110000;   
    dataToWrite |= level;

    // Write the data with level to the sensor.
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::WriteHeatCtrl);
    i2cbus.write(dataToWrite);
    i2cbus.endTransmission();
    // Return true to give feedback that the change went succesfully
    return true;
}


const double Si702X::getHumidity()const{
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::HumidityHoldMaster);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 2);

    int16_t humidity  = (i2cbus.read() << 8) + i2cbus.read();
    return ((125.0 * humidity) / 65536.0) - 6;
}


const double Si702X::getCelcius() const{
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::TemperatureHoldMaster);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 2);

    int16_t temp  = (i2cbus.read() << 8) + i2cbus.read();
    return ((175.72 * temp) / 65536.0) - 46.85;
}


const double Si702X::getKelvin() const{
    // Converts Celcius to Kelvin
    return getCelcius() + 273.15;
}


const double Si702X::getFahrenheit() const{
    // Converts Celcius to Fahrenheit.
    return getCelcius() * 9.0 / 5.0 + 32;
}


const double Si702X::getNewton() const{
    // The Newton scale is a temperature scale that sets a fixed zero point equal to the freezing point water. 
    // The scale uses both arithmetic progression and geometric progression. 
    // The scale defines 18 reference points and the corresponding "degrees of heat" and geometric progressions.

    // Converts Celcius to Newton.
    return getCelcius() * 0.33;
}


const double Si702X::getRankine() const{
    // Converts Celcius to Rankine.
    return ( getCelcius() + 273.15 ) * 9.0 / 5.0;
}


const double Si702X::getDelisle() const{
    // Converts Celcius to Delisle.
    return (100 - getCelcius()) * 3.0 / 2.0;
}


const double Si702X::getReaumer() const{
    // Converts Celcius to Reaumer.
    return getCelcius() * 0.8;
}


const double Si702X::getRomer() const{
    // Converts Celcius to Romer.
    return getCelcius() * 21.0 / 40.0 + 7.5;
}


const int16_t Si702X::getSerial() const{
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::SerialFirst);
    i2cbus.write(regAddress::SerialSecond);
    i2cbus.endTransmission(false);
    i2cbus.requestFrom(address, 2);

    return (i2cbus.read() << 8) + i2cbus.read();
}


const String Si702X::getDeviceID()const{
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::DeviceIdFirst);
    i2cbus.write(regAddress::DeviceIdSecond);
    i2cbus.endTransmission(false);

    i2cbus.requestFrom(address, 1);
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
    i2cbus.beginTransmission(address);
    i2cbus.write(regAddress::FirmwareFirst);
    i2cbus.write(regAddress::FirmwareSecond);
    i2cbus.endTransmission(false);

    i2cbus.requestFrom(address, 1);
    switch(i2cbus.read()){
        case 0x20:
            return "Firmware version 2.0";

        case 0xFF:
            return "Firmware version 1.0";

        default:
            return "Firmware version unknown";
    }
}