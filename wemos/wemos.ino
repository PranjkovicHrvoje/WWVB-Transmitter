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
  Serial.begin(9600);#include <ESP8266WiFi.h>
#include <NTPClientV2.h>
#include <WiFiUdp.h>
#include <Math.h>
#include <Time.h>


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
  int signal[59];

  get_time(signal);

  for(int i = 0; i<60;i++){
    if(signal[i] = 1){
      digitalWrite(16, HIGH);//fix for external LED
    }else{
      digitalWrite(16, LOW);//fix for external LED
    }

    Serial.print (i);
    Serial.print(" ");
    Serial.println(signal[i]);
  }
  
  delay(1000);
}

void get_time(int signal[59]
  ){
  timeClient.update();
  long int T = timeClient.getEpochTime();

  time_t epochTime = T;
  struct tm *current_time = localtime(&epochTime);  
  
  int second = current_time->tm_sec;
  int minute = current_time->tm_min;
  int hour = current_time->tm_hour; 
  int DoW = current_time->tm_wday+1;
  int DoM = current_time->tm_mday;
  int month = current_time->tm_mon+1;
  int year = current_time->tm_year;
  int DoY = current_time->tm_yday+1;
  int DST = current_time->tm_isdst;
  int iLY = is_leap_year(year+1);

  signal[0] = 2;
  signal[1] = ((minute/10)>>2)&1;
  signal[2] = ((minute/10)>>1)&1;
  signal[3] = ((minute/10)>>0)&1;
  signal[4] = 0;
  signal[5] = ((minute%10)>>3)&1;
  signal[6] = ((minute%10)>>2)&1;
  signal[7] = ((minute%10)>>1)&1;
  signal[8] = ((minute%10)>>0)&1;
  signal[9] = 2;
  signal[10] = 0;
  signal[11] = 0;
  signal[12] = ((hour/10)>>1)&1;
  signal[13] = ((hour/10)>>0)&1;
  signal[14] = 0;
  signal[15] = ((hour%10)>>3)&1;
  signal[16] = ((hour%10)>>2)&1;
  signal[17] = ((hour%10)>>1)&1;
  signal[18] = ((hour%10)>>0)&1;
  signal[19] = 2;
  signal[20] = 0;
  signal[21] = 0;
  signal[22] = ((DoY/10)>>1)&1;
  signal[23] = ((DoY/10)>>0)&1;
  signal[24] = 0;
  signal[25] = (((DoY/10)%10)>>3)&1;
  signal[26] = (((DoY/10)%10)>>2)&1;
  signal[27] = (((DoY/10)%10)>>1)&1;
  signal[28] = (((DoY/10)%10)>>0)&1;
  signal[29] = 2;
  signal[30] = ((DoY%10)>>3)&1;
  signal[31] = ((DoY%10)>>2)&1;
  signal[32] = ((DoY%10)>>1)&1;
  signal[33] = ((DoY%10)>>0)&1;
  signal[34] = 0;
  signal[35] = 0;
  signal[36] = 1;
  signal[37] = 0;
  signal[38] = 1;
  signal[39] = 2;
  signal[40] = 0; 
  signal[41] = 0;
  signal[42] = 0;
  signal[43] = 0;
  signal[44] = 0;  
  signal[45] = (((year/10)%10)>>3)&1;
  signal[46] = (((year/10)%10)>>2)&1;
  signal[47] = (((year/10)%10)>>1)&1;
  signal[48] = (((year/10)%10)>>0)&1;
  signal[49] = 2; 
  signal[50] = ((year%10)>>3)&1;
  signal[51] = ((year%10)>>2)&1;
  signal[52] = ((year%10)>>1)&1;
  signal[53] = ((year%10)>>0)&1;
  signal[54] = 0;  
  signal[55] = iLY;
  signal[56] = 0;
  signal[57] = DST;
  signal[58] = DST;
  signal[59] = 2;
}

int is_leap_year(int year){
  return ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0));
}

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





   
