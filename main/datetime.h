char daysOfTheWeek[7][12] = {"minggu", "senin", "selasal", "rabu", "kamis", "jumat", "sabtu"};

long epochNow = 0;

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void updateGlobalVarTime(){
  DateTime now = rtc.now();
  globalSec = now.second();
  globalMinute = now.minute();
  globalHour = now.hour();
  globalMonth = now.month();
  globalDay = now.day();
  globalYear = now.year();  
  globalEpoch = now.unixtime() + (7 * 3600);
}

void digitalClockDisplay() {
  DateTime now = rtc.now();
  Serial.print(now.unixtime());
  Serial.print(" - ");
  Serial.print(now.day());
  Serial.print('-');
  Serial.print(now.month());
  Serial.print('-');
  Serial.print(now.year());
  Serial.print(' ');
  Serial.print(now.hour());
  Serial.print(':');
  Serial.println(String(now.minute()) + " " + String(WiFi.status()));
}

void updateNtp() {
  if (WiFi.status() == WL_CONNECTED) {
    //start Time
    timeClient.begin();
    timeClient.update();
    digitalClockDisplay();
  }
}

void syncRtc(){
   if(!(time(nullptr) > 1618971013)){
      return;
    }
    
    int dayNtp = day(timeClient.getEpochTime() + offsetGmt);
    int monthNtp = month(timeClient.getEpochTime() + offsetGmt);
    int yearNtp = year(timeClient.getEpochTime() + offsetGmt);
    int hourNtp = hour(timeClient.getEpochTime() + offsetGmt);
    int secondNtp = second(timeClient.getEpochTime() + offsetGmt);
    int minuteNtp = minute(timeClient.getEpochTime() + offsetGmt);
    if(yearNtp < 2022){return;}
    rtc.adjust(DateTime(yearNtp, monthNtp, dayNtp, hourNtp, minuteNtp, secondNtp));
}

byte savedDay;
void eventDayChange(){
    if(globalDay != savedDay)
    {
      savedDay = globalDay;
      updateNtp();
      syncRtc();
    }
}

unsigned lastSec;
bool xState;
void eventSecondChange(){
  if(millis() - lastSec >= 1000){
    lastSec = millis();
    readSensor();
    detectChangePenyiraman();
    CheckSchedulePenyiraman();
    CheckSchedulePpm();
    displaySensor();
    displayInfo();
  }
}
