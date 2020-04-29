#ifndef SI7021_HPP
#define SI7021_HPP
#include <Arduino.h>
#include <Wire.h>



class Si702X{
private:
    static const uint8_t address{ 0x40 };
    TwoWire & _wire;
    enum regAddress: uint8_t{
        firmware1 = 0x84,
        firmware2 = 0xB8,
        heatCtrlW = 0x51,
        heatCtrlR = 0x11
    };
public:
    Si702X(TwoWire & wire = Wire);
    void reset();

    
    void setHeater(const bool & status);
    void setLvlHeater(const uint8_t & lvl);

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


    const String getDeviceID() const;
    const String getVersion() const;

};

#endif //SI7021_HPP