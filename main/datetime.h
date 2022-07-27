
char daysOfTheWeek[7][12] = {"minggu", "senin", "selasal", "rabu", "kamis", "jumat", "sabtu"};
byte savedDay, savedHour;
unsigned lastSec;
byte tickDetect = 100;
byte tickDisplaySensor = 100;
byte tickReadSensor = 100;
byte tickCheckSchedule = 100;
long epochNow = 0;
String dateStr;
byte tickReadSHT20;
int tickSendReport;
byte tickCheckInternet;
int tickDetectAll;
int tickCekPh;
int tickCekPpm;

void deviceBoot() {
  if (!limitDevice) {
    if (millis() > 30000) {
      limitDevice = true;
      Serial.println("Device Boot");
      setAktifitas("device started..");
    }
  }
}
void controlPompaPengisian() {
  if (sensFloat) {
    digitalWrite(RelayPompaPengisianPin, !HIGH);
  }
}

void controlPompaAduk() {
  if (isPhMixingBegin || isPpmMixingBegin) {
    digitalWrite(RelayPompaAdukPin, !HIGH);
  } else {
    digitalWrite(RelayPompaAdukPin, !LOW);
  }
}

void updateGlobalVarTime()
{
  DateTime now = rtc.now();
  //IF RTC WORK READ..
  if (isRtcConnected & (now.year() < 2040 && now.year() > 2020)) {
    globalSec = now.second();
    globalMinute = now.minute();
    globalHour = now.hour();
    globalMonth = now.month();
    globalDay = now.day();
    globalYear = now.year();
    globalEpoch = (now.unixtime());
  } else {
    //IF RTC NOT WORK READ FROM NTP
    globalEpoch = (timeClient.getEpochTime());
    globalSec = second(timeClient.getEpochTime());
    globalMinute = minute(timeClient.getEpochTime());
    globalHour = hour(timeClient.getEpochTime());
    globalMonth = month(timeClient.getEpochTime());
    globalDay = day(timeClient.getEpochTime());
    globalYear = year(timeClient.getEpochTime());
  }
  dateStr =  String(globalYear) + "/" + String(globalMonth) + "/" + String(globalDay) + " - " + String(globalHour) + ":" + String(globalMinute);
}

void debugData() {
  //readOutputStats
  bool phUpStats = !digitalRead(RelayPompaPhUpPin);
  bool phDownStats = !digitalRead(RelayPompaPhDownPin);
  bool ppmUpStats = !digitalRead(RelayPompaPpmUpPin);
  bool penyiramanStats = !digitalRead(RelayPompaPenyiramanPin);
  bool pengisianStats = !digitalRead(RelayPompaPengisianPin);
  bool sprayerStats = !digitalRead(RelaySprayerPin);

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

  _strData += " siram:";
  _strData += penyiramanStats;

  _strData += " isiTandon:";
  _strData += pengisianStats;

  _strData += "  date:";
  _strData += dateStr;

  _strData += " targetPh:";
  _strData += targetPh;

  _strData += " targetPpm:";
  _strData += targetPpm;

  _strData += "\n";

  _strData += "  Ppm:";
  _strData += sensPpm;

  _strData += "  Ph";
  _strData += sensPh;

  _strData += "  TempWater:";
  _strData += sensTempWater;

  _strData += "  Humid:";
  _strData += sensHumidity;

  _strData += "  TRoom";
  _strData += sensTempRoom;

  _strData += "  SchPen";
  _strData += dispPenyiramanStr;

  _strData += "  SchPpm:";
  _strData += dispPpmStr;

  //  _strData += "\n";
  //
  //  _strData += "  savedStatsSprayer:";
  //  _strData += savedStatsSprayer;
  //
  //  _strData += "  savedStatsTargetPpm:";
  //  _strData += savedStatsTargetPpm;
  //
  //  _strData += "  savedStatsPenyiraman:";
  //  _strData += savedStatsPenyiraman;
  //
  //  _strData += "  savedStatsTargetPpm:";
  //  _strData += savedStatsTargetPpm;
  //
  //  _strData += "  savedStatsPengisian:";
  //  _strData += savedStatsPengisian;
  //
  //  _strData += "\n";
  //
  //  _strData += "  savedStatsPhUp:";
  //  _strData += savedStatsPhUp;
  //
  //  _strData += "  savedStatsPhDown:";
  //  _strData += savedStatsPhDown;
  //
  //  _strData += "  savedStatsPpmUp:";
  //  _strData += savedStatsPpmUp;
  //
  //  _strData += "  savedStatsPh:";
  //  _strData += savedStatsPh;
  //
  //  _strData += "  savedStatsPpm:";
  //  _strData += savedStatsPpm;
  //
  //  _strData += "\n";
  //
  //  _strData += "  savedStatsHumidity:";
  //  _strData += savedStatsHumidity;
  //
  //  _strData += "  savedStatsTempRoom:";
  //  _strData += savedStatsTempRoom;
  //
  //  _strData += "  savedStatsWaterTemp:";
  //  _strData += savedStatsWaterTemp;

  _strData += "  internetStats:";
  _strData += internetStats;

  _strData += "  WiFI.status:";
  _strData += WiFi.status();

  //  _strData += "  usSprayer:";
  //  _strData += updateSprayerStats;

  //  _strData += "\n";
  //  _strData += "  usTargetPpm:";
  //  _strData += updateTargetPpmStats;

  //  _strData += "  usWaterTemp:";
  //  _strData += updateWaterTempStats;
  //
  //  _strData += "  usWaterTemp:";
  //  _strData += updateWaterTempStats;
  //
  //  _strData += "  usTempRoom:";
  //  _strData += updateTempRoomStats;
  //
  //  _strData += "  usPh:";
  //  _strData += updatePhStats;
  //
  //  _strData += "  usPpm:";
  //  _strData += updatePpmStats;
  //
  //  _strData += "  usHumidity:";
  //  _strData += updateHumidityStats;
  //
  //  _strData += "  usPpmUp:";
  //  _strData += updatePpmUpStats;

  _strData += "\n";

  _strData += "  usPenyiraman:";
  _strData += updatePenyiramanStats;

  _strData += "  usPengisian:";
  _strData += updatePengisianStats;

  _strData += "  usPhUp:";
  _strData += updatePhUpStats;

  _strData += "  usPhDown:";
  _strData += updatePhDownStats;

  _strData += "  dispPenyirStr:";
  _strData += dispPenyiramanStr;

  _strData += "  akt1:";
  _strData += aktifitasStr;

  _strData += "  akt2:";
  _strData += aktifitasStr2;

  _strData += "  akt3:";
  _strData += aktifitasStr3;

  _strData += "  tickSendReport:";
  _strData += tickSendReport;

  _strData += "  uptime:";
  _strData += millis() / 1000;

  _strData += "\n";

  _strData += "  int_cekPh:";
  _strData += intervalCekPh;

  _strData += "  int_cekPpm:";
  _strData += intervalCekPpm;

  _strData += "  int_report:";
  _strData += intervalSendReport;

  _strData += "  int_detectAll:";
  _strData += intervalDetectAll;

  _strData += "  marginPh:";
  _strData += batasMarginPh;

  _strData += "  marginPpm:";
  _strData += batasMarginPpm;

  _strData += "  isPpmMixingBegin:";
  _strData += isPpmMixingBegin;

  _strData += "  isPhtMixingBegin:";
  _strData += isPhMixingBegin;

  Serial.println();
  Serial.println(_strData);
}

void updateNtp()
{
  if (internetStats)
  {
    //start Time
    timeClient.begin();
    timeClient.update();
  }
}

void syncRtc() {
  if (WiFi.status() != 3) return;
  int dayNtp = day(timeClient.getEpochTime());
  int monthNtp = month(timeClient.getEpochTime());
  int yearNtp = year(timeClient.getEpochTime());
  int hourNtp = hour(timeClient.getEpochTime());
  int secondNtp = second(timeClient.getEpochTime());
  int minuteNtp = minute(timeClient.getEpochTime());
  if (yearNtp >= 2022) {
    rtc.adjust(DateTime(yearNtp, monthNtp, dayNtp, hourNtp, minuteNtp, secondNtp));
  }

}

void eventDayChange()
{
  if (globalDay != savedDay)
  {
    savedDay = globalDay;
    updateNtp();
    syncRtc();
  }
}

void eventHourChange()
{
  if (globalHour != savedHour)
  {
    savedHour = globalHour;
    updateAllGrafik();
    if (!isRtcConnected) {
      updateNtp();
    }
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
    displayTime();
    updateGlobalVarTime();
    displayInfo();
    debugData();
    deviceBoot();
    sendStatsAktifitas();
    tickCheckSchedule ++;
    tickReadSensor ++;
    tickReadSHT20 ++;
    tickDisplaySensor ++;
    tickCheckInternet++;
    tickDetect ++;
    tickSendReport++;
    tickDetectAll++;
    tickCekPpm++;
    tickCekPh++;
    detectForFilling();
    sendFcmNotif();
    controlPompaAduk();
    controlPompaPengisian();

    if (tickCekPh > (int) intervalCekPh) {
      isPhMixingBegin = true;
      limSendNotifModePh = false;
      limSendCalculatePhInfo = false;
      tickCekPh = 0;
    }

    if (tickCekPpm > (int) intervalCekPpm) {
      isPpmMixingBegin = true;
      limSendNotifModePpm = false;
      limSendCalculatePpmInfo = false;
      tickCekPpm = 0;
    }

    if (tickDetectAll > intervalDetectAll) {
      tickDetectAll = 0;
      Serial.println(" >> DetectAll");
      detectChangeAllSensor();
    }

    if (tickSendReport > intervalSendReport) {
      tickSendReport = 0;
    }

    if (tickReadSHT20 > 20)
    {
      vTaskResume(TaskHandle_2);
      tickReadSHT20 = 0;
    }

    if (tickCheckSchedule > 5)
    {
      tickCheckSchedule = 0;
      CheckSchedulePenyiraman();
      CheckSchedulePpm();
    }

    //tickReadSensor
    if (tickReadSensor > 10)
    {
      tickReadSensor = 0;
      vTaskResume(TaskHandle_3); // check internet
      readStatus = true;
      readSensor();
    }

    //tickDisplaySensor
    if (tickDisplaySensor > 5)
    {
      tickDisplaySensor = 0;
      sensPhDisplay = sensPh;
      sensPpmDisplay = sensPpm;
      displaySensor();
    }

    //detectChange every x tick
    if (tickDetect > 5)
    {
      tickDetect = 0;
      detectChangePenyiraman();
      detectChangePengisian();
      //update ph pump stats
      if (!isPhMixingBegin) {
        detectChangePhUp();
        detectChangePhDown();
      }
      //update ppm pump stats
      if (!isPpmMixingBegin) {
        detectChangePpmUp();
      }
      //update target value
      detectChangeTargetPpm();
    }
  }
}
