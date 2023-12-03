#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>

static const String ssid_ = "ssid"; // WiFi ssid
static const String password_ = "password"; // WiFi password
static const String server_ = "server ip"; // Server host name
static const String port_ = "port"; //Server port number

SoftwareSerial mySerial(2,3); //RX, TX
boolean connected = false;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  /*/
  Serial.println("ESP8266 wifi 연결");
  mySerial.println("AT+RST");
  delay(3000);
  emptyOutSerialBuffer();

  while(1) { //WiFi 연결
    if(connectWiFi()) {
      connected = true;
      break;
    }
    delay(1000);
  }
  emptyOutSerialBuffer();//*/
}

void loop() {
  if(mySerial.available()) {
    Serial.write(mySerial.read());
    return;
  }
  
  int voltage = 0;

  StaticJsonDocument<200> json; // JSON 형태로 메세지를 만듦
  json["Voltage"] = 0;

  String parsedJsonToString;
  serializeJson(json, parsedJsonToString);
  mySerial.println("AT+CIPCLOSE");
  delay(100);
  emptyOutSerialBuffer();

  boolean TCPconnected = TCPconnect();
  emptyOutSerialBuffer();

  if(TCPconnected) {
    TCPsend(parsedJsonToString, parsedJsonToString.length());
    delay(100);
  }
}

boolean connectWiFi() {  
  String cmd = "AT+CWJAP=\"" + ssid_ + "\",\"" + password_ + "\"";  
  mySerial.println(cmd);
  Serial.println(cmd);
  delay(5000);
  if(mySerial.find("WIFI CONNECTED")) {  
    Serial.println("Wifi 연결 완료."); 
    return true;  
  }  
  else {  
    Serial.println("Wifi 연결 중");
    return false;  
  }  
}

boolean TCPconnect() { // TCP 연결
  String cmd = "AT+CIPSTART=\"TCP\",\"" + server_ + "\"," + port_;

  Serial.println(cmd);
  mySerial.println(cmd);

  if (!mySerial.find("OK")) {
    Serial.println("TCP connect ERROR");
    return false;
  }

  return true;
}

void TCPsend(String msg, byte msgLength) { // 서버에 전송
  String cmd = "POST /Nfc HTTP/1.1\r\nHost: " + server_ + "\r\nContent-Type:  application/json\r\nContent-Length: " + msgLength + "\r\n\r\n" + msg + "\r\n";

  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  delay(100);
  Serial.print(cmd);
  mySerial.print(cmd);
}

void emptyOutSerialBuffer() { // 버퍼 비우기
  while(mySerial.read() > 0) {
  }
}