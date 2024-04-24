#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
//ประกาศตัวแปรต่างๆสำหรับกำหนดค่าการทำงานของโปรแกรม
String ssid = "ParSafe-Setup"; // ชื่อ WiFi
String password = "88888888"; // รหัส WiFi
const char* mqtt_server = "broker.netpie.io"; // Broker MQTT
const int mqtt_port = 1883; // MQTT port
const char* mqtt_Client = "41e37469-27fb-4765-ba18-f09795a16d1e"; // MQTT Client ID
const char* mqtt_username = "iRhhkwrbeSYDEdCLSADQAUU9hphqgP1L"; // MQTT Token
const char* mqtt_password = "gS8JtQqzrkreDEjWL29NjjxR5PXTLM2t";// MQTT Secret

WiFiClient espClient;
PubSubClient client(espClient);


// function สำหรับการเปลี่ยนการเชื่อมต่อเครือข่าย WiFi
void ChangeWiFi(String SSID, String PASSWORD) {
    WiFi.disconnect();
    ssid = SSID;
    password = PASSWORD;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      int count;
      delay(100);
      Serial.print(".");
      count++;
      if (count > 120){
        WiFi.disconnect();
        break;
      }
    }
    Serial.println("");
    Serial.println("WiFi Connected!");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.print("Connect To : ");
    Serial.println(ssid);
    delay(100);
    Serial.println("-----------------------");
}

// ฟังชั่นสำหรับรับ Input จาก Node-red
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic >| ");
  Serial.print(topic);
  Serial.print(" |<  :  ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println("#  ");
  Serial.print(message);
  Serial.println("  ;");
  JsonDocument doc;
  deserializeJson(doc, message);

  //ถ้า topic เป็น "@msg/WiFi/change" จะทำกราเรียกใช้ฟังชั่น ChangeWiFI
  if (String(topic) == "@msg/WiFi/change"){
    String C_ssid = doc["ssid"];
    String C_password = doc["password"];
    ChangeWiFi(C_ssid, C_password);
  }

  //ถ้า topic เป็น "@msg/Board/askPassCode" จะทำการส่ง state "askPassCode" และ UserID ไปที่ Arduino Mega2560
  if (String(topic) == "@msg/Board/askPassCode"){
    String destination = doc["destination"];
    String msg = "{\"state\" : \"askPassCode\", \"destination\" : \"" + destination + "\"}";

    Serial2.println(msg);

  }
}

//function สำหรับ setup การเชื่อมต่อ WiFi และการเชื่อมต่อ Netpie ผ่าน Protocol MQTT
void NetpieSetup(){

    Serial.println("-----------------");
    Serial.println("");
    Serial.println("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      int count;
      delay(100);
      Serial.print(".");
      count++;
      if (count > 120){
        WiFi.disconnect();
        break;
      }
    }
    Serial.println("");
    Serial.println("WiFi Connected!");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.print("Connect To : ");
    Serial.println(ssid);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    Serial.println("-----------------------");
    delay(100);
}

//เมื่อการเชื่อมต่อ Netpie มีปัญหาจะทำการเชื่อมต่อใหม่ด้วยการเรียกใช้ฟังชั่นนี้
void reconnectToNetpie(){
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)){
      Serial.println("NetPie Connected");

      client.subscribe("@msg/test1");
      client.subscribe("@msg/WiFi/change");
      client.subscribe("@msg/Board/askPassCode");
      
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      Serial.println("try again in 1 seconds");
      delay(1000);
    }
  }
}

//ฟังชั่นสำหรับวนซ้ำการทำงานของการเชื่อมต่อ WiFi และเชื่อมต่อ Netpie
void NetPieLoop() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        Serial.println("Trying To Reconnect WiFi: ");

        while (WiFi.status() != WL_CONNECTED) {
          int count;
          delay(100);
          Serial.print(".");
          count++;
          if (count > 120){
            WiFi.disconnect();
            break;
          }
        }
         Serial.println("");
         Serial.println("WiFi Connected!");
    }

    if (!client.connected()) {
        Serial.println("Netpie Not Connect");
        reconnectToNetpie();
    }
}