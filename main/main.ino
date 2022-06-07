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

#include "TimeLib.h"
#include "RTClib.h"
RTC_DS3231 rtc;

String uid = "OoQcNgqaBqchpWRjwe9PRw6n3tb2";
#define WIFI_SSID "AMI"
#define WIFI_PASSWORD "admin.admin"

byte dispIndex;
unsigned int offsetGmt = 3600 * 7;//set parameter variable
String schPenyiramanStr, schPpmStr, manualPhDownStr, manualPhUpStr, modePhStr, modePpmStr, targetPhStr, targetPpmStr, manualPpmUpStr = "";
float sensPh, sensPpm, sensHumidity, sensTempWater, sensTempRoom, sensWaterTemp;
float targetPh, targetPpm;
float savedStatsPh, savedStatsPpm, savedStatsHumidity, savedStatsTempRoom, savedStatsWaterTemp;
bool savedStatsPengisian, savedStatsPpmUp, savedStatsPhUp, savedStatsPhDown;
bool updatePengisianStats, updatePpmStats, updatePhStats, updateHumidityStats, updateWaterTempStats, updateTempRoomStats, updatePpmUpStats, updatePhUpStats, updatePhDownStats;
bool pompaPhUpStats, pompaPpmUpStats, pompaPhDownStats, pompaPengisianStats,pompaPenyiramanStats;
bool RelayPompaPenyiraman, RelayPhUp, RelayPhDown, RelayPpm;
byte RelayPompaPhUpPin = 14;
byte RelayPompaPhDownPin = 5;
byte RelayPompaPpmUpPin = 17;
byte RelayPompaPenyiramanPin = 18;
byte RelayPompaPengisianPin = 15;
byte floatStatus;
byte floatSensorPin = 16;

String myData = "";
bool savedStatsPenyiraman, penyiramanStats, updatePenyiramanStats;
byte globalSec, globalMinute, globalHour, globalDay, globalMonth;
int globalYear;
unsigned long globalEpoch;

#include "preferences.h"
#include "lcd.h"
#include "sensor.h"
#include "response_output_from_firebase.h"
#include "firebase.h"
#include "push_grafik_firebase.h"
#include "push_sensor_change.h"
#include "datetime.h"
#include "wifi_event.h"
#include "preferences_start.h"
#include "at_serial.h"

void setup()
{
  //SerialBegin
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  //rtc
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }

  //preferences
  preferences.begin("my-app", false);

  //set output mode
  pinMode(RelayPompaPenyiramanPin, OUTPUT);
  pinMode(RelayPompaPhUpPin, OUTPUT);
  pinMode(RelayPompaPhDownPin, OUTPUT);
  pinMode(RelayPompaPpmUpPin, OUTPUT);
  pinMode(floatSensorPin, INPUT);
  pinMode(RelayPompaPengisianPin, HIGH);

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

  Serial.println("begin to setup");
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 , 512 );
#endif
  begin_stream();

  updateNtp();
  parsingInternalData();
}

void loop()
{
//  Serial.println(digitalRead(RelayPompaPhUpPin));
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
