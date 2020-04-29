#include <Arduino.h>
#include "Si7021.hpp"

Si7021 sensor = Si7021();
void setup() {
    Wire.begin();
    Serial.begin(9600);
}

void loop() {
    delay(1000);
    Serial.print("Temperature ");
    Serial.println( sensor.getCelcius() );
    Serial.print("Humidity  ");
    Serial.println( sensor.getHumidity() );
    // Serial.println(sensor.getDeviceID());
    // Serial.println(sensor.getVersion());
}
