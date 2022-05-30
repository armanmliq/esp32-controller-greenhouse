#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h> 
#include <addons/RTDBHelper.h>


//set parameter variable
String schPenyiramanStr,schPpmStr,manualPhDownStr,manualPhUpStr,modePhStr,modePpmStr,targetPhStr,targetPpmStr,manualPpmUpStr = "";

bool RelayPhUp,RelayPhDown,RelayPpm;
byte RelayPhUpPin= 1;
byte RelayPhDownPin = 2;
byte RelayPpmPin = 3;

String myData ="";


#include "preferences.h" 
#include "datetime.h" 
#include "firebase.h"
#include "on_wifi_timeout.h"

void setup()
{
  pinMode(RelayPhUpPin,OUTPUT);
  pinMode(RelayPhDownPin,OUTPUT);
  pinMode(RelayPpmPin,OUTPUT);     

  //SerialBegin
  Serial.begin(115200); 

  //StartWiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long _startMillis = millis();
  while (WiFi.status() != WL_CONNECTED & millis() - _startMillis < 6000)
  {
    Serial.print(".");
    delay(300);
  }
  
  timeClient.begin();
  timeClient.update();
  updateNtp(); 
  setupFirebase();
  
  preferences.begin("my-app", false); 
}

void loop()
{ 
  readPreferences();
  timeClient.update();
  updateNtp();
  delay(1000);

}
