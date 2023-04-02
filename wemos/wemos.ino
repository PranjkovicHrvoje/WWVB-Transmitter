#include <ESP8266WiFi.h>
//#include <NTPClient.h>
#include <NTPClientV2.h>
#include <WiFiUdp.h>
#include <Math.h>


// Network credentials
const char* ssid = "DESKTOP-J4EC1OB 4992";
const char* password = "hrvojekeks";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//void get_time();
 
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
  int signal[59];

  get_time(signal);  
  delay(1000);
}

void get_time(int signal[59]
  ){
  timeClient.update();
  int T = timeClient.getEpochTime();
  int second = timeClient.getMinutes();
  int minute = timeClient.getMinutes();
  int hours = timeClient.getHours(); 
  int day = timeClient.getDay();
  int month = 1; // missing function
  int year = timeClient.getYears();
  int dayOfYear = day_of_year(day, month, year);
  int daylightSavingsTime = is_daylight_savings_time(day, month, year);
  

  Serial.println(hours);
  }

int day_of_week(long day, long month, long year){
  long a = (14 - month) / 12;
  long y = year + 4800 - a;
  long m = month + 12 * a - 3;
  long d = day + (153 * m + 2) / 5 + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045;

  return (d + 1) % 7;
  }  

int day_of_year(int day, int month, int year){
  int cum_days_in_months[]={
    0,
    31,
    59,
    90,
    120,
    151,
    181,
    212,
    243,
    273,
    304,
    334
    };
    return (cum_days_in_months[month - 1] + day + (is_leap_year(year)) & month > 2);
  } 

int is_leap_year(int year){
  return ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0));
  }

int is_daylight_savings_time(int day, int month, int year){
  if (month <= 2 || 12 <= month) return 0;
  if (4 <= month && month <= 10) return 1;
  int dow = timeClient.getDay();
    if (month == 3) {
        return (day - timeClient.getDay() > 7);
    } else {
        // month == 11
        return (day - timeClient.getDay() <= 0);
    }
  }





   
