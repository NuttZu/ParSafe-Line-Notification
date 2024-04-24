// include Module
#include "Process.h"

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  SonicSetup();
  LCD_SetUp();
  PassCodeSetup();

  Serial.println("void setup success");
}

unsigned long Sonic_time;

void loop() {
  // ส่งค่าจากเซ็นเซอร์ UltraSonic ไปที่ ESP32 ทุกๆ 1 วิ
  if (millis() - Sonic_time > 1000) {
    int sonic_distance = GetSonic();
    Sonic_time = millis();
    String msg = "{\"state\" : \"SendSonic\" , \"Value\" : \"" + String(sonic_distance) + "\"}";
    Serial2.println(msg);
    Serial.println(msg);
  }

  // เงื่อนไขสำหรับปลดล็อคประตู
  if (keypad_state == 1) {
    digitalWrite(doorlock, 0);
    delay(5000);
    keypad_state=0;
  }
  if (keypad_state == 0) {
    digitalWrite(doorlock, 1);
  }

  NuttGetKey();
  
  //Serial.println(GetSonic());
  
  //------------------------------------------
  //รับข้อมูลจาก ESP32
  if (Serial2.available() != 0)
  {
    Serial.println("Serial Received: ");
    String var = Serial2.readString();
    Serial.println(var);
    
    JsonDocument doc;
    deserializeJson(doc, var);

    //Recieve Json from esp32  
    String state = doc["state"];

    //คืนค่ารหัสผ่านไปที่ ESP32 ถ้า state เป็น  askPassCode
    if (state == "askPassCode"){
      String destination = doc["destination"];
      String msg = "{\"state\" : \"askPassCode\", \"passCode\" : \"" + getPassCode() + "\", \"destination\" : \"" + destination + "\"}";
      Serial.println(msg);
      Serial2.println(msg);
    }
  }

  //Sent Data
  // if (Serial.available() != 0)
  // {
  //   Serial.println("Serial Sent: ");
  //   String var = Serial.readString();
  //   Serial2.println(var);
  // }
  //------------------------------------------
  
}