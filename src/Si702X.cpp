#include "Si702X.hpp"

Si702X::Si702X(TwoWire & wire): _wire(wire){

}



void Si702X::reset(){

}

// void Si7021::setHeater(const bool & status);

// }


void Si702X::setLvlHeater(const uint8_t & lvl = 0){

}


const double Si702X::getHumidity()const{
    _wire.beginTransmission(address);
    _wire.write(0xE5);
    _wire.endTransmission();
    _wire.requestFrom(address, 2);

    int16_t humidity  = (_wire.read() << 8) + _wire.read();
    return ((125 * humidity) / 65536.0) - 6;
}

const double Si702X::getCelcius() const{
    _wire.beginTransmission(address);
    _wire.write(0xE3);
    _wire.endTransmission();
    _wire.requestFrom(address, 2);

    int16_t temp  = (_wire.read() << 8) + _wire.read();
    return ((175.72 * temp) / 65536.0) - 46.85;
}
const double Si702X::getKelvin() const{
    // Converts Celcius to Kelvin
    return getCelcius() + 273.15;
}
const double Si702X::getFahrenheit() const{
    // Converts Celcius to Fahrenheit.
    return getCelcius() * 9 / 5 + 32;
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
    return ( getCelcius() + 273.15 ) * 9 / 5;
}

const double Si702X::getDelisle() const{
    // Converts Celcius to Delisle.
    return (100 - getCelcius()) * 3 / 2;
}

const double Si702X::getReaumer() const{
    // Converts Celcius to Reaumer.
    return getCelcius() * 0.8;
}

const double Si702X::getRomer() const{
    // Converts Celcius to Romer.
    return getCelcius() * 21 / 40 + 7.5;
}


const String Si702X::getDeviceID()const{
    _wire.beginTransmission(address);
    _wire.write(0xFC);
    _wire.write(0xC9);
    _wire.endTransmission(false);

    _wire.requestFrom(address, 1);
    switch(_wire.read()){
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
    _wire.beginTransmission(address);
    _wire.write(0x84);
    _wire.write(0xB8);
    _wire.endTransmission(false);

    _wire.requestFrom(address, 1);
    switch(_wire.read()){
        case 0x20:
            return "Firmware version 2.0";

        case 0xFF:
            return "Firmware version 1.0";

        default:
            return "Firmware version unknown";
    }
}