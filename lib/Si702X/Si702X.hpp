#ifndef SI7021_HPP
#define SI7021_HPP
#include <Arduino.h>
#include <Wire.h>



class Si702X{
private:
    static const byte address{ 0x40 };
    TwoWire & i2cbus;

    enum regAddress: byte{
        HumidityHoldMaster = 0xE5,
        HumidityNoHoldMaster = 0xF5,

        TemperatureHoldMaster = 0xE3,
        TemperatureNoHoldMaster = 0xF3,

        Reset = 0xFE,

        SerialFirst = 0xFA,
        SerialSecond = 0x0F,

        DeviceIdFirst = 0xFC,
        DeviceIdSecond = 0xC9,

        FirmwareFirst = 0x84,
        FirmwareSecond = 0xB8,

        WriteHeatCtrl = 0x51,
        ReadHeatCtrl = 0x11,

        WriteUser = 0xE6,
        ReadUser = 0xE7
    };

public:

    // Constructor
    // If no argument is given the standard "Wire" bus will be used.
    Si702X(TwoWire & i2cbus = Wire);



    // Config

    // Resets sensor settings
    byte reset();

    // Enable heater on sensor.
    // If heater level is not configured it will not heat up (see setHeater(level) ).
    byte enableHeater();

    // Disables heater on sensor.
    // Does not change configured level of heater
    byte disableHeater();

    // Set sensor heater.
    // The heater can be set to a level between 0 - 15. The higher number the higher the heat.
    // A level above 15 will return 5 and not change the heater level.
    // Heater needs to be enabled to take effect (see enableHeater() ).
    // If no argument is given the level will be 0 i.e. off
    byte setHeater(const byte & level = 0 );

    // Changes resolution of RH and temperature measurements.
    // Measurement Resolution:
    //     RH       Temp
    // 00: 12 bit   14 bit - Default
    // 01: 8 bit    12 bit
    // 10: 10 bit   13 bit
    // 11: 11 bit   11 bit
    byte setResolution(const bool & a = 0, const bool & b = 0);



    // Humidity
    
    // Get relative humidity from sensor
    float getRH() const;



    // Temperature
    
    // Get temperature reading from sensor in Celcius
    float getCelcius() const;

    // Get temperature reading from sensor in Kelvin
    float getKelvin() const;

    // Get temperature reading from sensor in Fahrenheit
    float getFahrenheit() const;

    // Get temperature reading from sensor in Newton
    float getNewton() const;

    // Get temperature reading from sensor in Rankine
    float getRankine() const;

    // Get temperature reading from sensor in Delisle
    float getDelisle() const;

    // Get temperature reading from sensor in Reaumur
    float getReaumur() const;

    // Get temperature reading from sensor in Romer
    float getRomer() const;



    // Information

    // Get serial number from sensor
    int16_t getSerial() const;

    // Get device id from sensor
    String getDeviceID() const;

    // Get version from sensor
    String getVersion() const;

};

#endif //SI7021_HPP