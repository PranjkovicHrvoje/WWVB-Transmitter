#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Network credentials
const char* ssid = "DESKTOP-J4EC1OB 4992";
const char* password = "hrvojekeks";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
 
void setup() {
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
  // Connecting to WiFi
  WiFi.hostname("Name");
  WiFi.begin(ssid, password);  
  
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}
 
void loop() {

  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(BUILTIN_LED, LOW);
  }else{
    digitalWrite(BUILTIN_LED, HIGH);
  }
  //Requesting time
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}
