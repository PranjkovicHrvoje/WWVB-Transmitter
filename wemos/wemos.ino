#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#define DEBUG


unsigned long T = 0;

// Network credentials
const char* ssid = "Ejovrh";
const char* password = "12345679";


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "hr.pool.ntp.org");


void setup() {
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(13, INPUT_PULLUP);

  //Connecting to WiFi
  WiFi.hostname("Name");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    Serial.print(".");
  }
  digitalWrite(BUILTIN_LED, LOW);
  Wire.begin();
  timeClient.begin();
}

void loop() {
  if (digitalRead(13) == LOW) {
    Serial.println("Started");
    timeClient.update();
    T = timeClient.getEpochTime();
    Wire.beginTransmission(27);

    byte a, b, c, d;
    a = (T >> 24) & 0xFF;
    b = (T >> 16) & 0xFF;
    c = (T >> 8) & 0xFF;
    d = T & 0xFF;
    Wire.write(a);
    Wire.write(b);
    Wire.write(c);
    Wire.write(d);

    Wire.endTransmission();

    Serial.println(T);
    Serial.println("Sent");

    delay(1000);
    T = 0;
  }  
}
