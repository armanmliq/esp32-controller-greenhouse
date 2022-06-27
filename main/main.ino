#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

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
#include <EEPROM.h>
#define PH_PIN 35
float voltage;
DFRobot_PH ph;

bool isRtcConnected = false;

//temp
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 23
OneWire wireSuhu(ONE_WIRE_BUS);
DallasTemperature sensorSuhu(&wireSuhu);

String uid = "GB12AsiGsmPhPRVeOlFD9Uv6oHm1";
#define WIFI_SSID "AMI"
#define WIFI_PASSWORD "admin.admin"

byte dispIndex;
unsigned int offsetGmt = 3600 * 7;//set parameter variable
String schPenyiramanStr, schPpmStr, manualPhDownStr, manualPhUpStr, targetPhStr, targetPpmStr, manualPpmUpStr = "";
float sensPh, sensPpm, sensTempWater;
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
unsigned int maksIntervalOn = 1000 * 30; //30 detik
unsigned int maksIntervalOff = 1000 * 30; ///30 detik
unsigned long maksIntervalLimit = 1000 * 60 * 120; // 120 minute
unsigned long limitDecreasePpm;
unsigned long deleteGrafikSecondExceedFrom = 2 * 24 * 3600;
int deleteGrafikItemAtOnce = 10;
int intervalUpdateGrafik = 1;
byte RelayPompaPhUpPin = 14;
byte RelayPompaPhDownPin = 5;
byte RelayPompaPpmUpPin = 20;
byte RelayPompaPenyiramanPin = 18;
byte RelayPompaPengisianPin = 19;
byte RelaySprayerPin = 32;
byte floatSensorPin = 16;

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
byte updateRate = 4;
String dispTimeStr;
float sensPhDisplay, sensPpmDisplay;

bool limitDevice;

void setAktifitas(String str) {
  aktifitasStr = str;
  updateAktifitasStats = true;
}

bool readSHT;

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

  //sht20
  setupSht20();

  //set output mode
  pinMode(RelayPompaPengisianPin, OUTPUT);
  pinMode(RelayPompaPenyiramanPin, OUTPUT);
  pinMode(RelayPompaPhUpPin, OUTPUT);
  pinMode(RelayPompaPhDownPin, OUTPUT);
  pinMode(RelayPompaPpmUpPin, OUTPUT);
  pinMode(RelaySprayerPin, OUTPUT);
  pinMode(floatSensorPin, INPUT_PULLUP);

  //begin wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long timeout_wifi_m = millis();

  //on disconnect
  WiFi.onEvent(wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(wifi_connected, SYSTEM_EVENT_STA_CONNECTED);

  //wait or timeout
  while (WiFi.status() != WL_CONNECTED & millis() - timeout_wifi_m < 20000) {
    lcd.setCursor(0, 0);
    lcd.print("CONNECTING WIFI ");
    lcd.setCursor(0, 1);
    lcd.print(WIFI_SSID);
  }
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 2);
    lcd.print("CONNECTED");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("NOT CONNECTED");
  }
  xTaskCreate(
    taskOne,          /* Task function. */
    "TaskOne",        /* String with name of task. */
    20000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    0,                /* Priority of the task. */
    0
  );

  xTaskCreate(
    readSHT20,          /* Task function. */
    "readSHT20",        /* String with name of task. */
    2000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    0,                /* Priority of the task. */
    0
  );

  Serial.println("begin to setup");
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  timeClient.setTimeOffset(7 * 3600);
#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 , 512 );
#endif
  begin_stream();
  updateNtp();
  parsingInternalData();
}

void loop()
{
  if (!readStatus) {
    delay(5);
  }
  limitAll();
  eventDayChange();
  eventSecondChange();
  detectForPhMixing();
  detectForPpmMixing();
  serial();
}

void taskOne( void * parameter )
{
  for (;;) {
    handleGrafik();
    vTaskDelay(10);
    sendAllStats();
  }
  vTaskDelete( NULL );
}
