char daysOfTheWeek[7][12] = {"minggu", "senin", "selasal", "rabu", "kamis", "jumat", "sabtu"};
byte savedDay;
unsigned lastSec;
byte tickDetect = 100;
byte tickDisplaySensor = 100;
byte tickReadSensor = 100;
byte tickCheckSchedule = 100;
long epochNow = 0;
String dateStr;

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void deviceBoot() {
  if (!limitDevice) {
    if (millis() > 30000) {
      limitDevice = true;
      Serial.println("Device Boot");
      setAktifitas("Device Boot...");
    }
  }
}
void updateGlobalVarTime()
{
  //IF RTC WORK READ..
  if (isRtcConnected) {
    DateTime now = rtc.now();
    globalSec = now.second();
    globalMinute = now.minute();
    globalHour = now.hour();
    globalMonth = now.month();
    globalDay = now.day();
    globalYear = now.year();
    globalEpoch = (now.unixtime());
  } else {
    //IF RTC NOT WORK READ FROM NTP
    //Get a time structure
    globalEpoch = (timeClient.getEpochTime());
    struct tm *ptm = gmtime ((time_t *)&globalEpoch);
    globalSec = (timeClient.getSeconds());
    globalMinute = (timeClient.getMinutes());
    globalHour = (timeClient.getHours());
    globalMonth = (ptm->tm_mon);
    globalDay = (ptm->tm_mday);
    globalYear = (ptm->tm_year + 1900);
  }

  dateStr =  String(globalYear) + "/" + String(globalMonth) + "/" + String(globalDay) + " - " + String(globalHour) + ":" + String(globalMinute);
}



void debugData() {
  //readOutputStats

  bool phUpStats = digitalRead(RelayPompaPhUpPin);
  bool phDownStats = digitalRead(RelayPompaPhDownPin);
  bool ppmUpStats = digitalRead(RelayPompaPpmUpPin);
  bool penyiramanStats = digitalRead(RelayPompaPenyiramanPin);
  bool pengisianStats = digitalRead(RelayPompaPengisianPin);
  bool sprayerStats = digitalRead(RelaySprayerPin);

  Serial.println();
  String _strData = "";
  _strData += "sensFloat:";
  _strData += sensFloat;

  _strData += " sprayer:";
  _strData += sprayerStats;

  _strData += "  phUp:";
  _strData += phUpStats;

  _strData += "  phDown:";
  _strData += phDownStats;

  _strData += " ppmUp:";
  _strData += ppmUpStats;

  _strData += " penyir:";
  _strData += penyiramanStats;

  _strData += " pengi:";
  _strData += pengisianStats;

  _strData += "  epoch:";
  _strData += globalEpoch;

  _strData += " targetPh:";
  _strData += targetPh;

  _strData += "  Ph";
  _strData += sensPh;

  _strData += " targetPpm:";
  _strData += targetPpm;

  _strData += "  Ppm";
  _strData += sensPpm;

  _strData += "  \nTempWater";
  _strData += sensTempWater;

  _strData += "  Humid";
  _strData += sensHumidity;

  _strData += "  TRoom";
  _strData += sensTempRoom;

  _strData += "  SchPen";
  _strData += dispPenyiramanStr;

  _strData += "  SchPpm";
  _strData += dispPpmStr;


  Serial.println();
  Serial.println(_strData);
}
void updateNtp() {
  if (WiFi.status() == WL_CONNECTED)
  {
    //start Time
    timeClient.begin();
    timeClient.update();
  }
}

void syncRtc() {
  if (!(time(nullptr) > 1618971013)) {
    return;
  }
  if (WiFi.status() != 3) return;
  int dayNtp = day(timeClient.getEpochTime());
  int monthNtp = month(timeClient.getEpochTime());
  int yearNtp = year(timeClient.getEpochTime());
  int hourNtp = hour(timeClient.getEpochTime());
  int secondNtp = second(timeClient.getEpochTime());
  int minuteNtp = minute(timeClient.getEpochTime());
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

void eventSecondChange()
{

  ph.calibration(voltage, sensTempWater);
  tds.setTemperature(sensTempWater);  //Masukan Nilai Suhu sebagai Temperature Compensation
  tds.update();  //Proses Perhitungan Nilai TDS, Jangan Memakan Perintah "Delay()" agar tidak mengganggu proses kalkulasi nilai tds
  if (millis() - lastSec >= 1000)
  {
    lastSec = millis();
    updateGlobalVarTime();
    readSensor();
    displayInfo();
    debugData();
    deviceBoot();
    tickCheckSchedule++;

    if (tickCheckSchedule > 5)
    {
      tickCheckSchedule = 0;
      CheckSchedulePenyiraman();
      CheckSchedulePpm();
    }

    tickReadSensor ++;
    if (tickReadSensor > 10)
    {
      tickReadSensor = 0;
      readStatus = true;
      readSensor();
      readStatus = false;
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
    if (tickDetect > 5)
    {
      tickDetect = 0;
      Serial.println("detectChangeAllSensor");
      //      detectChangeAllSensor();
      detectForFilling();
    }
  }
}
