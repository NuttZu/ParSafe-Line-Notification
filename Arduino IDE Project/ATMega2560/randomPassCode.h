#include <EEPROM.h>

#define doorlock 46

//function สำหรับการเขียน eeprom ลงบนบอร์ด Arduino
void setPassCode(byte value1, byte value2, byte value3, byte value4){
  EEPROM.put(0, value1);
  EEPROM.put(1, value2);
  EEPROM.put(2, value3);
  EEPROM.put(3, value4);
}

//Function สำหรับดึงข้อมูลจาก eeprom บนบอร์ด Arduino ออกมา
String getPassCode() {
  String passCode;
  for (int i=0; i<4; i++) {
    byte x;
    EEPROM.get(i, x);
    passCode += String(x);
  }
  return passCode;
}

//function สำหรับ setup รหัสของ keypad หากใน eeprom ไม่มีรหัสใดๆ
void PassCodeSetup(){
  pinMode(doorlock, OUTPUT);
  digitalWrite(doorlock, 1);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("------------------------------------");
  String passCode = getPassCode();
  Serial.println("passCode = " + passCode);
  if (passCode == "0000") {
    byte value1 = byte(random(0,9));
    byte value2 = byte(random(0,9));
    byte value3 = byte(random(0,9));
    byte value4 = byte(random(0,9));
    setPassCode(value1, value2, value3, value4);
  }
}