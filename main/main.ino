#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h> 
#include <addons/RTDBHelper.h>
#define WIFI_SSID "AMI"
#define WIFI_PASSWORD "admin.admin"
  
unsigned int offsetGmt = 3600 * 7;

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
#include "on_disconnect.h" 
#include "preferences_start.h" 

void setup()
{
  preferences.begin("my-app", false); 
  readPreferences(); 
 
  //set output mode
  pinMode(RelayPhUpPin,OUTPUT);
  pinMode(RelayPhDownPin,OUTPUT);
  pinMode(RelayPpmPin,OUTPUT);     

  //SerialBegin
  Serial.begin(115200);  

  //update internal
  parsingInternalData();


  //begin wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long timeout_wifi_m = millis();
  
  //on disconnect
  WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  
  //wait or timeout  
  while(WiFi.status() != WL_CONNECTED & millis() - timeout_wifi_m < 20000){
    Serial.println("waiting connect");
  }

  setupFirebase();
  updateNtp();    
  
}

void loop()
{ 
  CheckSchedulePenyiraman();
  CheckSchedulePpm();
  updateNtp();
  delay(1000); 
}
