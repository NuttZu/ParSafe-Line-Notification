#include "KeyPadPassWord.h"

LiquidCrystal_I2C lcd(0x27,20,4);

void LCD_EnterPassword() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Password :");
  lcd.setCursor(0,2);
  lcd.print(">>  ");
}

void LCD_SetUp(){
  lcd.init();
  lcd.backlight();
  LCD_EnterPassword();
}

const byte ROWS = 4; // จำนวนแถสแนวนอน
const byte COLS = 3; // จำนวนแถวแนวตั้ง
char keys[ROWS][COLS] = { // แมททริกส์สำหรับกำหนดอักขระจองแต่ละปุ่มบน Keypad
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //ขาที่ต่อกับ keypad ในส่วนแถวแนวนอน
byte colPins[COLS] = {4, 3, 2}; //ขาที่ต่อกับ keypad ในส่วนแถวแนวตั้ง

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int KeypadArray[4] = { 0 };
byte arrayIndex = 0;

String PasswordKeypad;


// Main Function for keypad
char NuttGetKey(){
  char customKey = customKeypad.getKey();
  

  // รับ input จาก keypad
  if (customKey == '0' || customKey == '1' || customKey == '2' || customKey == '3' || customKey == '4' || customKey == '5' || customKey == '6' || customKey == '7' || customKey == '8' || customKey == '9') {
    if (arrayIndex < 4) {
      KeypadArray[arrayIndex] = customKey - '0';
      lcd.setCursor((arrayIndex+3),2);
      lcd.print(customKey);
      arrayIndex++;
    }
  }

  // ปุ่ม backspace ของ keypad (ปุ่ม '#')
  if (customKey == '#') {
    if(arrayIndex != 0){
      lcd.setCursor((arrayIndex+2),2);
      lcd.print(" ");
      arrayIndex--;
    }
  }

  // เมื่อกดปุ่มครบ 4 ตัว จะเช็กว่าทั้งสี่ตัวนี้ ตรงกับรหัส 4 ตัว ใน eeprom หรือไม่
  if (arrayIndex == 4) {
    PasswordKeypad = "";
    for (int i = 0; i < arrayIndex; i++){
      PasswordKeypad += KeypadArray[i];
    }
    Keypad_Check(PasswordKeypad);
    Serial.println("User Input : " + PasswordKeypad);
    arrayIndex = 0;
    lcd.setCursor((3),2);
    lcd.print("    ");
  }

}

 
