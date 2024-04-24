#include "ConnectToNetPie.h"

//กำหนดขา RX, TX
#define RX 16
#define TX 17

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);

  NetpieSetup();
}

void loop()
{
  //--------------------------------------------------------
  //รับข้อมูลจาก Arduino Mega2560
  if (Serial2.available() != 0)
  {
    Serial.println("Serial Received: ");
    String var = Serial2.readString();
    Serial.println(var);

    JsonDocument doc;
    deserializeJson(doc, var);

    String state = doc["state"];

    //ถ้า state เป็น askPassCode จะส่ง รหัส Keypad และ UserID ไปที่ Node-Red
    if (state == "askPassCode"){
      String destination = doc["destination"];
      String passCode = doc["passCode"];
      String raw_msg = "{\"state\" : \"askPassCode\", \"passCode\" : \"" + passCode + "\", \"destination\" : \"" + destination + "\"}";
      const char* msg = raw_msg.c_str();
      client.publish("@msg/Line/Notify/Specific", msg);
    }

    //ถ้า state เป็น SendSonic จะส่งค่าที่ได้จาก UltraSonic ไปที่ Node-Red
    if (state == "SendSonic") {
      String Value = doc["Value"];
      String raw_msg = "{\"state\" : \"SendSonic\", \"Value\" : \"" + Value + "\"}";
      const char* msg = raw_msg.c_str();
      client.publish("@msg/UltraSonic", msg);
    }
  }

  //Sent Data
  // if (Serial.available() != 0)
  // {
  //   Serial.println("Serial Sent: ");
  //   String var = Serial.readString();
  //   Serial2.println(var);
  // }

  //--------------------------------------------------------
  //เรียกใช้ function NetPieLoop เพื่อวนซ้ำการทำงานของ library “ConnectToNetpie.h”
  NetPieLoop();
  //ใช้ function client.loop เพื่อให้ ESP32 เชื่อมต่อ MQTT เมื่อจบ 1 loop
  client.loop();
}