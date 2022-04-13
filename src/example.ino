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
