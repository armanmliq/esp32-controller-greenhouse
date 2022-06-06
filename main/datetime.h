char daysOfTheWeek[7][12] = {"minggu", "senin", "selasal", "rabu", "kamis", "jumat", "sabtu"};

long epochNow = 0;

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void printRtc() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");
  delay(3000);
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
    DateTime now = rtc.now();
    if(now.day() != savedDay)
    {
      savedDay = now.day();
      updateNtp();
      syncRtc();
    }
}

unsigned lastSec;
void eventSecondChange(){
  if(millis() - lastSec >= 1000){
    lastSec = millis();
    CheckSchedulePenyiraman();
    CheckSchedulePpm();
    printRtc();
    displaySensor();
  }
}
