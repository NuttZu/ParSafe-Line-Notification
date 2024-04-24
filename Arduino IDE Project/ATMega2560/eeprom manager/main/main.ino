#include "Arduino.h"
#include "HardwareSerial.h"
#include <EEPROM.h>

void setPassCode(byte value1, byte value2, byte value3, byte value4){
  EEPROM.put(0, value1);
  EEPROM.put(1, value2);
  EEPROM.put(2, value3);
  EEPROM.put(3, value4);
}

void clearPassCode() {
  for (int i; i>4; i++) {
    EEPROM.write(i, 0);
  }
}

void setup() {
  Serial.begin(115200);
  //clearPassCode();
  setPassCode(2,2,8,8);
  setPassCode(0,0,0,0);
  Serial.println("End, setup();");
}

void loop() {

}