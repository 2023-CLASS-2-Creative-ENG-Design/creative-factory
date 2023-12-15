#include <Servo.h> 
#include <SoftwareSerial.h>
#include <SPI.h>
#include <string.h>
#include <ArduinoJson.h>

#define SERVOPINH  5 
#define SERVOPINV  6

#define dtime   50  
#define tol   50   

static const String ssid_ = "Hyotaek";
static const String password_ = "20110517";
static const String server_ = "ec2-3-36-133-190.ap-northeast-2.compute.amazonaws.com";
static const String port_ = "8080";

SoftwareSerial mySerial(2,3); //RX, TX

boolean connected = false;

Servo horizontal;
int servoh = 90;   

int servohLimitHigh = 175;  
int servohLimitLow = 5;     


Servo vertical;     
int servov = 90;     

int servovLimitHigh = 100;  
int servovLimitLow = 20;    

 
const int ldrlt = A0; //Upper left
const int ldrrt = A1; //Upper right
const int ldrld = A2; //Lower left
const int ldrrd = A3; //Lower right

float vout = 0.0;

float vin = 0.0;

float R1 = 30000.0;

float R2 = 7500.0;

int SPIN = A4;

int value = 0;
int iter = 0;

void setup()
{
  Serial.begin(9600);
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);

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
  }//*/
  mySerial.println("AT+CIPCLOSE");
  emptyOutSerialBuffer();
  pinMode(SPIN, INPUT);
  Serial.print("Voltage:");

}

void loop() {
  iter++;
  int lt = analogRead(ldrlt); 
  int rt = analogRead(ldrrt); 
  int ld = analogRead(ldrld); 
  int rd = analogRead(ldrrd); 
  
  int avt = (lt + rt) / 2; 
  int avd = (ld + rd) / 2; 
  int avl = (lt + ld) / 2; 
  int avr = (rt + rd) / 2; 
  
  int dvert = avt - avd; 
  int dhoriz = avl - avr;


  if (-1*tol > dvert || dvert > tol) 
 {
  if (avt < avd)
  {
    servov = --servov;
     if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
  }
  else if (avt > avd) {
    servov= ++servov;
    if (servov < servovLimitLow) {
      servov = servovLimitLow;
    Serial.println(servov);
  }
  }
  vertical.write(servov); 
  }

 
  if (-1*tol > dhoriz || dhoriz > tol) {
    if (avl < avr) {
      servoh = --servoh;
      if (servoh < servohLimitLow) {
        servoh = servohLimitLow;
      }
  }
  else if (avl > avr)
  {
    servoh = ++servoh;
     if (servoh > servohLimitHigh)
     {
     servoh = servohLimitHigh;
     }
  }
  else if (avl = avr)
  {
    
  }
  horizontal.write(servoh); 
  }

  value = analogRead(SPIN);

  vout = (value * 5.0) / 1024.0;

  vin = vout / ( R2 / ( R1 + R2) );


  Serial.print("V: ");

  Serial.println(vin,2);
  if(iter >= 200) {
    horizontal.detach();
    vertical.detach();
    mySerial.println("AT+CIPCLOSE");
    delay(10);
    emptyOutSerialBuffer();
    boolean TCPconnected = TCPconnect();
    if(TCPconnected) {
      StaticJsonDocument<200> json; // JSON 형태로 메세지를 만듦
      json["voltage"] = vin;

      String parsedJsonToString;
      serializeJson(json, parsedJsonToString);
      TCPsend(parsedJsonToString, parsedJsonToString.length());
    }
    delay(300);
    mySerial.println("AT+CIPCLOSE");
    delay(10);
    emptyOutSerialBuffer();
    iter = 0;
    horizontal.attach(SERVOPINH);
    vertical.attach(SERVOPINV);
  }
  delay(dtime);
}


boolean connectWiFi() {  
   //mySerial.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"" + ssid_ + "\",\"" + password_ + "\"";  
  mySerial.println(cmd);
  Serial.println(cmd);
  delay(5000);
  if(mySerial.find("OK")) {  
   Serial.println("Wifi 연결 완료."); 
   return true;  
  }  
  else {  
   Serial.println("Wifi 연결 중");  
   return false;  
  }  
}

boolean TCPconnect() {
  String cmd = "AT+CIPSTART=\"TCP\",\"" + server_ + "\"," + port_;

  Serial.println(cmd);
  mySerial.println(cmd);

  if(!mySerial.find("OK")) {
    Serial.println("TCP connect ERROR");
    return false;
  }

  return true;
}

boolean TCPsend(String msg, byte msgLength) {
  String cmd = "POST /demo HTTP/1.1\r\nHost: " + server_ + "\r\nContent-Type: application/json\r\nContent-Length: " + msgLength + "\r\n\r\n" + msg + "\r\n";

  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  delay(100);
  Serial.println(cmd);
  mySerial.println(cmd);
}

void emptyOutSerialBuffer() {
  while(mySerial.read() > 0) {
  }
}