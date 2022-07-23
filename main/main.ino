#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

bool internetStats = true;
String aktivitasPh, aktivitasPpm;
String body = "Pompa tandon dimatikan";
String title = "Pengisian air baku selesai";


#define WIFI_SSID "AMI"
#define WIFI_PASSWORD "admin.admin"

//#define WIFI_SSID "AQILA"
//#define WIFI_PASSWORD "balabala"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include "ModbusMaster.h" //https://github.com/4-20ma/ModbusMaster
#define MAX485_RE_NEG  4 //D4 RS485 has a enable/disable pin to transmit or receive data. Arduino Digital Pin 2 = Rx/Tx 'Enable'; High to Transmit, Low to Receive
#define Slave_ID       1
#define RX_PIN      16 //RX2 
#define TX_PIN      17  //TX2  
ModbusMaster modbus;

#include "TimeLib.h"
#include "RTClib.h"
RTC_DS3231 rtc;

//tds
float tdsValue;
#include "RddTDS.h"
RddTDS tds;
#define TdsSensorPin 34

//ph
#include "DFRobot_PH.h"
#define PH_PIN 35
float voltage;
DFRobot_PH ph;

bool isRtcConnected = 1;

//temp
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 23
OneWire wireSuhu(ONE_WIRE_BUS);
DallasTemperature sensorSuhu(&wireSuhu);

String uid = "GB12AsiGsmPhPRVeOlFD9Uv6oHm1";

//ESP PING
#include <ESP32Ping.h>
const char* remote_host = "www.google.com";
unsigned long checkInet_m;

bool isSendNotif;
byte dispIndex;
unsigned int offsetGmt = 3600 * 7;//set parameter variable
String schPenyiramanStr, schPpmStr, manualPhDownStr, manualPhUpStr, targetPhStr, targetPpmStr, manualPpmUpStr = "";
float sensPh, sensPpm, sensTempWater, dummyPh, dummyPpm;
bool modeDummyPh;
bool modeDummyPpm;

String initialPenyiraman, initialPengisian;

double sensHumidity, sensTempRoom;

String modePhStr = "OTOMATIS";
String modePpmStr = "OTOMATIS";

float targetPh = 5;
float targetPpm = 800;

float batasMarginPh = 0.5;

unsigned long intervalLimit = 1000 * 60 * 120; //Limit lama on pompa (safety off) 120 menit
unsigned int intervalOn = 1000 * 6; // lama on
unsigned int intervalOff = 1000 * 2; // lama off
bool readStatus;
int maksPpm = 1200;

unsigned int intervalPh;
unsigned int intervalPpm;
unsigned int intervalOnPpm = intervalOn;
unsigned int intervalOffPpm = intervalOff;
unsigned int intervalOnPh = intervalOn;
unsigned int intervalOffPh = intervalOff;
unsigned int maksIntervalOn = 1000 * 120; //30 detik
unsigned int maksIntervalOff = 1000 * 120; ///30 detik
unsigned long maksIntervalLimit = 1000 * 60 * 120; // 120 minute
unsigned long limitDecreasePpm;
unsigned long deleteGrafikSecondExceedFrom = 2 * 24 * 3600;

float intervalCekPpm = 60 * 20; // 20 menit
float intervalCekPh = 60 * 20; // 20 menit
float intervalDetectAll = 60 * 2; // 2 menit
float intervalSendReport = 60 * 20;
float intervalNotif = 60 * 5;

unsigned long intervalNotifOtomatisPh = intervalNotif * 1000;
unsigned long intervalNotifOtomatisPpm = intervalNotif * 1000;

float batasMarginPpm = 50;

int deleteGrafikItemAtOnce = 10;
int intervalUpdateGrafik = 1;

byte RelayPompaPhUpPin = 26;
byte RelayPompaPhDownPin = 25;
byte RelayPompaPpmUpPin = 19;
byte RelayPompaPenyiramanPin = 18;
byte RelayPompaPengisianPin = 15;
byte RelaySprayerPin = 14;
byte RelayValvePenyiramanPin = 13;
byte RelayPompaPpmDownPin = 23;

byte floatSensorPin = 39;

float savedStatsPh, savedStatsPpm, savedStatsHumidity, savedStatsTempRoom, savedStatsWaterTemp, savedStatsTargetPpm;
bool savedStatsPengisian, savedStatsPpmUp, savedStatsPhUp, savedStatsPhDown = true;
bool sprayerStats = true;
bool updatePengisianStats, updatePpmStats, updatePhStats, updateHumidityStats, updateWaterTempStats, updateTempRoomStats, updatePpmUpStats, updatePhUpStats, updatePhDownStats, updateTargetPpmStats, updateAktifitasStats  = true;
bool pompaPhUpStats, pompaPpmUpStats, pompaPhDownStats, pompaPengisianStats, pompaPenyiramanStats, targetPpmStats;
String aktifitasStr;
bool RelayPompaPenyiraman, RelayPhUp, RelayPhDown, RelayPpm, RelaySprayer;
byte sensFloat;

String myData = "";
bool savedStatsPenyiraman, penyiramanStats, updatePenyiramanStats;
byte globalSec, globalMinute, globalHour, globalDay, globalMonth;
int globalYear;
unsigned long globalEpoch;
String intervalOnStr, intervalOffStr, intervalLimitStr, batasMarginPhStr;

bool savedStatsSprayer;
bool updateSprayerStats;
bool lastStatsLimitSprayer;
unsigned long lastMillisSprayer;

byte tick;
byte updateRate = 2;
String dispTimeStr;
float sensPhDisplay, sensPpmDisplay;

bool limitDevice;
String aktifitasStr2, aktifitasStr3 = "";

unsigned long mCalcPh;
unsigned long mCalcPpm;

void setAktifitas(String str) {
  if (internetStats) {
    if (aktifitasStr.length() > 1) {
      if (aktifitasStr2.length() > 1) {
        aktifitasStr3 = str;
      } else {
        aktifitasStr2 = str;
      }
    } else {
      aktifitasStr = str;
    }
    updateAktifitasStats = true;
  }
}

bool readSHT;
bool reConnectStream;

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
TaskHandle_t TaskHandle_4;

#include "notif_fcm_api.h"
#include "lcd.h"
#include "json.h"
#include "preferences.h"
#include "response_output_from_firebase.h"
#include "firebase.h"
#include "push_grafik_firebase.h"
#include "push_sensor_change.h"
#include "sensor.h"
#include "datetime.h"
#include "wifi_event.h"
#include "at_serial.h"

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 60

void setup()
{

  //SerialBegin
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  //rtc
  isRtcConnected = true;
  if (!rtc.begin()) {
    isRtcConnected = false;
    Serial.println("Couldn't find RTC");
  }

  tdsSetup();

  //preferences
  preferences.begin("my-app", false);
  parsingInternalData();


  //sht20
  setupSht20();

  //set output mode
  pinMode(RelayPompaPengisianPin, OUTPUT);
  pinMode(RelayPompaPenyiramanPin, OUTPUT);
  pinMode(RelayPompaPhUpPin, OUTPUT);
  pinMode(RelayPompaPhDownPin, OUTPUT);
  pinMode(RelayPompaPpmUpPin, OUTPUT);
  pinMode(RelaySprayerPin, OUTPUT);
  pinMode(RelayValvePenyiramanPin, OUTPUT);
  pinMode(RelayPompaPpmDownPin, OUTPUT);
  pinMode(floatSensorPin, INPUT_PULLUP);
  allRelayOff();


  //begin wifi
  WiFi.disconnect();
  delay(2000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long timeout_wifi_m = millis();

  //on disconnect
  WiFi.onEvent(wifi_connected, SYSTEM_EVENT_STA_CONNECTED);

  //display initial
  lcd.setCursor(0, 0);
  lcd.print("***agroX Teknologi**");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("connecting wifi..");
  delay(2000);
  lcd.setCursor(0, 2);
  lcd.print("SSID: ");
  lcd.print(WIFI_SSID);
  delay(2000);

  //wait or timeout
  while (WiFi.status() != WL_CONNECTED & millis() - timeout_wifi_m < 20000) {
    esp_task_wdt_reset();
  }



  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 3);
    lcd.print("Connected..");

  } else {
    lcd.setCursor(0, 3);
    lcd.print("NOT CONNECTED");
  }

  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  timeClient.setTimeOffset(7 * 3600);
  timeClient.begin();
  timeClient.update();
  setup_firebase();
#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 , 512 );
#endif
  begin_stream();

  //checkInternet
  xTaskCreate(checkInternet, "checkInternet", 8000, NULL, 0, &TaskHandle_3);

  Serial.println("======> begin to setup <=======");
  lcd.clear();
  xTaskCreatePinnedToCore(readSHT20, "readSHT20", 2000, NULL, 0, &TaskHandle_2, 0);
  xTaskCreate(sendNotif, "sendNotif", 5000, NULL, 0, &TaskHandle_4);
  vTaskSuspend(TaskHandle_4);
  xTaskCreatePinnedToCore(taskOne, "taskOne", 40000, NULL, 0, &TaskHandle_1, 0);
  vTaskSuspend(TaskHandle_1);

  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}

void loop()
{
  esp_task_wdt_reset();
  limitAll();
  eventDayChange();
  eventHourChange();
  eventSecondChange();
  detectForPhMixing();
  detectForPpmMixing();
  serial();
  sendAllStats();
}

void sendNotif(void * parameter)
{
  for (;;) {
    if (!internetStats) {
      vTaskSuspend(TaskHandle_4);
    }
    vTaskSuspend(TaskHandle_3);
    sendFcmNotif();
    vTaskResume(TaskHandle_3);
  }
}

void taskOne( void * parameter )
{
  for (;;)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void readSHT20(void * parameter ) {
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    uint8_t result = modbus.readInputRegisters(0x01, 2);
    if (getResultMsg(&modbus, result))
    {
      double res_dbl = (float)modbus.getResponseBuffer(0) / 10.00F;
      sensTempRoom = res_dbl;
      sensTempRoom = roundToOneDecPoint(sensTempRoom);
      res_dbl = (float)modbus.getResponseBuffer(1) / 10.00F;
      sensHumidity = res_dbl;
      sensHumidity = roundToOneDecPoint(sensHumidity);
      if (sensTempRoom <= 0 || sensTempRoom >= 100) sensTempRoom = 25;
      if (sensHumidity <= 0 || sensHumidity > 100) sensHumidity = 80;
      vTaskSuspend(TaskHandle_2);
    }
    vTaskSuspend(TaskHandle_2);
  }
  vTaskDelete( NULL );
}

void checkInternet(void * parameter) {
  for (;;)
  {
    if (WiFi.status() != 3) {
      Serial.println("disable check inet at wifi disconnect..");
      internetStats = false;
      vTaskSuspend(TaskHandle_3);
    }
    if (Ping.ping(remote_host)) {
      internetStats = true;
      Serial.println(">>>>> ping success!");
    } else {
      internetStats = false;
      Serial.println(">>>>> ping failed!");
    }
    vTaskSuspend(TaskHandle_3);
  }
}
