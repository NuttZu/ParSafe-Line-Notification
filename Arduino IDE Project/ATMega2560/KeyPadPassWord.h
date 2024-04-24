#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

bool keypad_state;


//ฟังชั่นสำหรับเช็ค input จาก user
void KeyPad_Correct(){
  Serial.println("Password Correct");
  randomSeed(millis());

  byte value1 = byte(random(0,9));
  byte value2 = byte(random(0,9));
  byte value3 = byte(random(0,9));
  byte value4 = byte(random(0,9));
  setPassCode(value1, value2, value3, value4);

  keypad_state = 1;
}

//ถ้า input จาก user ผิด จะแสดงผล "Password Wrong" ทาง Serial port
void KeyPad_Wrong(){
  Serial.println("Password Wrong");
}

//ฟังชั่นสำหรับเช็กว่า input จาก user ตรงกับค่าใน eeprom หรือไม่
void Keypad_Check(String Input_passCode){
  String eeprom_passcode = getPassCode();
  if (Input_passCode == eeprom_passcode) {
    KeyPad_Correct();
  }
  else {
    KeyPad_Wrong();
  }
}