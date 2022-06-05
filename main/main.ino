#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h> 
#include <addons/RTDBHelper.h>


String uid = "OoQcNgqaBqchpWRjwe9PRw6n3tb2";
#define WIFI_SSID "AMI"
#define WIFI_PASSWORD "admin.admin" 


unsigned int offsetGmt = 3600 * 7;

//set parameter variable
String schPenyiramanStr,schPpmStr,manualPhDownStr,manualPhUpStr,modePhStr,modePpmStr,targetPhStr,targetPpmStr,manualPpmUpStr = "";
float sensPh,sensPpm,sensHumidity,sensTemperature;

bool RelayPhUp,RelayPhDown,RelayPpm;
byte RelayPhUpPin= 1;
byte RelayPhDownPin = 2;
byte RelayPpmPin = 3;
String myData ="";

#include "preferences.h" 
#include "datetime.h" 
#include "firebase.h" 
#include "wifi_event.h" 
#include "preferences_start.h" 
#include "push_grafik_firebase.h"
#include "at_serial.h" 
void setup()
{
  //SerialBegin
  Serial.begin(115200);  
  
  preferences.begin("my-app", false); 
  readPreferences(); 
  

  //set output mode
  pinMode(RelayPhUpPin,OUTPUT);
  pinMode(RelayPhDownPin,OUTPUT);
  pinMode(RelayPpmPin,OUTPUT);     



  //update internal
  parsingInternalData(); 
  
  //begin wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long timeout_wifi_m = millis();
  
  //on disconnect
  WiFi.onEvent(wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  WiFi.onEvent(wifi_connected, SYSTEM_EVENT_STA_CONNECTED);   
  //wait or timeout  
  while(WiFi.status() != WL_CONNECTED & millis() - timeout_wifi_m < 20000){
    Serial.println("waiting connect");
    delay(2000);
  }
  xTaskCreate(
                    taskOne,          /* Task function. */
                    "TaskOne",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */
                   
  updateNtp();    
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
}

void loop()
{


  setupFirebaseAtWifiDisconnect();
  serial();
    CheckSchedulePenyiraman();
    CheckSchedulePpm();
  updateNtp();
  delay(1000); 
}

void taskOne( void * parameter )
{
 
    for(;;){
        handleGrafik();
        //Serial.println("Hello from task 1");
        yield();
        delay(1000);
    } 
    vTaskDelete( NULL );
 
}
