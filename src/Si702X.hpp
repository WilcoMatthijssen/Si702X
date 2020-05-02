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
    Si702X(TwoWire & i2cbus = Wire);
    void reset();



    // add resolution
    
    bool enableHeater();
    bool disableHeater();
    bool setHeater(const byte & level = 0 );

    // Humidity
    const double getHumidity() const;

    // Get temperature
    const double getCelcius() const;
    const double getKelvin() const;
    const double getFahrenheit() const;
    const double getNewton() const;
    const double getRankine() const;
    const double getDelisle() const;
    const double getReaumer() const;
    const double getRomer() const;

    const int16_t getSerial() const;
    const String getDeviceID() const;
    const String getVersion() const;

};

#endif //SI7021_HPP