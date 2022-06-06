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
LiquidCrystal_I2C lcd(0x27,20,4);  

#include "TimeLib.h"
#include "RTClib.h"
RTC_DS3231 rtc;

String uid = "OoQcNgqaBqchpWRjwe9PRw6n3tb2";
#define WIFI_SSID "AMI"
#define WIFI_PASSWORD "admin.admin" 

unsigned int offsetGmt = 3600 * 7;

//set parameter variable
String schPenyiramanStr,schPpmStr,manualPhDownStr,manualPhUpStr,modePhStr,modePpmStr,targetPhStr,targetPpmStr,manualPpmUpStr = "";
float sensPh,sensPpm,sensHumidity;
float targetPh,targetPpm;


bool RelayPompaPenyiraman,RelayPhUp,RelayPhDown,RelayPpm;
byte RelayPhUpPin= 1;
byte RelayPhDownPin = 2;
byte RelayPpmPin = 3;
byte RelayPompaPenyiramanPin = 4;
String myData ="";

#include "preferences.h" 
#include "response_output_from_firebase.h"
#include "firebase.h" 
#include "lcd.h"
#include "datetime.h" 
#include "wifi_event.h" 
#include "preferences_start.h" 
#include "push_grafik_firebase.h"
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
//    Serial.flush();
//    while (1) delay(10);
  }


  //preferences
  preferences.begin("my-app", false);  
    
  //set output mode
  pinMode(RelayPhUpPin,OUTPUT);
  pinMode(RelayPhDownPin,OUTPUT);
  pinMode(RelayPpmPin,OUTPUT);     
  pinMode(RelayPompaPenyiramanPin,OUTPUT);  

  
  //begin wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long timeout_wifi_m = millis();
  
  //on disconnect
  WiFi.onEvent(wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  WiFi.onEvent(wifi_connected, SYSTEM_EVENT_STA_CONNECTED);   
  //wait or timeout  
  while(WiFi.status() != WL_CONNECTED & millis() - timeout_wifi_m < 20000){
   lcd.setCursor(0,0);
   lcd.print("connecting to ");
   lcd.setCursor(0,1);
   lcd.print(WIFI_SSID);
  }
  xTaskCreate(
    taskOne,          /* Task function. */
    "TaskOne",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL
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
    stream.setBSSLBufferSize(2048 , 512 /* Tx in bytes, 512 - 16384 */);
  #endif
    begin_stream();

  updateNtp();     
  parsingInternalData(); 
}

void loop()
{
  eventDayChange();
  eventSecondChange();
  serial();


}

void taskOne( void * parameter )
{ 
    for(;;){
        //handleGrafik(); 
        yield();
        delay(1000);
    } 
    vTaskDelete( NULL );
 
}
