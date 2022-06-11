char daysOfTheWeek[7][12] = {"minggu", "senin", "selasal", "rabu", "kamis", "jumat", "sabtu"};
byte savedDay;
unsigned lastSec;
byte tickDetect = 100;
byte tickDisplaySensor = 100;
byte tickReadSensor = 100;
long epochNow = 0;

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void updateGlobalVarTime() {
  DateTime now = rtc.now();
  globalSec = now.second();
  globalMinute = now.minute();
  globalHour = now.hour();
  globalMonth = now.month();
  globalDay = now.day();
  globalYear = now.year();
  globalEpoch = now.unixtime() + (7 * 3600);
}
void printStatsPompa() {
  bool phUpStats = digitalRead(RelayPompaPhUpPin);
  bool phDownStats = digitalRead(RelayPompaPhDownPin);
  bool ppmUpStats = digitalRead(RelayPompaPpmUpPin);
  bool penyiramanStats = digitalRead(RelayPompaPenyiramanPin);
  bool pengisianStats = digitalRead(RelayPompaPengisianPin);
  Serial.println("phUp:" + String(phUpStats) + " phDown:" + String(phDownStats) + " ppmUp:" + String(ppmUpStats) + " penyiraman:" + String(penyiramanStats) + " pengisian:" + String(pengisianStats));
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

void syncRtc() {
  if (!(time(nullptr) > 1618971013)) {
    return;
  }
  int dayNtp = day(timeClient.getEpochTime() + offsetGmt);
  int monthNtp = month(timeClient.getEpochTime() + offsetGmt);
  int yearNtp = year(timeClient.getEpochTime() + offsetGmt);
  int hourNtp = hour(timeClient.getEpochTime() + offsetGmt);
  int secondNtp = second(timeClient.getEpochTime() + offsetGmt);
  int minuteNtp = minute(timeClient.getEpochTime() + offsetGmt);
  if (yearNtp < 2022) {
    return;
  }
  rtc.adjust(DateTime(yearNtp, monthNtp, dayNtp, hourNtp, minuteNtp, secondNtp));
}



void eventDayChange() {
  if (globalDay != savedDay)
  {
    savedDay = globalDay;
    updateNtp();
    syncRtc();
  }
}


void eventSecondChange() {
  if (millis() - lastSec >= 1000)
  {
    lastSec = millis();
    readSensor();
    displayInfo();
    printStatsPompa();
    
    tickReadSensor ++;
    if (tickReadSensor > 10) {
      tickReadSensor = 0;
      readSensor();
    }

    tickDisplaySensor ++;
    if (tickDisplaySensor > 10) {
      tickDisplaySensor = 0;
      sensPhDisplay = sensPh;
      sensPpmDisplay = sensPpm;
      displaySensor();
    }

    //detectChange every x tick
    tickDetect ++;
    if (tickDetect > 10)
    {
      tickDetect = 0;
      Serial.println("detectChangeAllSensor");
      detectChangeAllSensor();
    }
  }
}
