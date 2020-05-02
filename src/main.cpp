#include <Arduino.h>
#include "Si702X.hpp"

Si702X sensor = Si702X();
void setup() {
    Wire.begin();
    Serial.begin(9600);
}

void loop() {
    delay(1000);
    Serial.print("Temperature ");
    Serial.println( sensor.getCelcius() );
    Serial.print("Humidity ");
    Serial.println( sensor.getHumidity() );
    Serial.println(sensor.getSerial());
    Serial.println(sensor.getDeviceID());
    Serial.println(sensor.getVersion());
}
