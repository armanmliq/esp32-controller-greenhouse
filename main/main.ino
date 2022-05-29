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
#define API_KEY "AIzaSyBwu8sXyJOM-uu-NB9kfjNkIrnDkMKPoco" 
#define DATABASE_URL "https://esp32-7fe29-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app 
#define USER_EMAIL "armanmaliq92@gmail.com"
#define USER_PASSWORD "armanmaliq92@gmail.com"

FirebaseData fbdo;
FirebaseData streamScheduler;
FirebaseData streamParameter;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
String MainPathsetParameter = "/users/OoQcNgqaBqchpWRjwe9PRw6n3tb2/set_parameter/";
String MainPathScheduler = "/users/OoQcNgqaBqchpWRjwe9PRw6n3tb2/scheduler";
String childPathSetParameter[11] = {"set_dosing_pump_ppm", "set_humidity","set_mode_ppm","set_ph","set_ppm", "set_mode_irigasi" ,"set_mode_ph","scheduler_ppm_str", "scheduler_jadwal_penyiraman","set_dosing_pump_ph_down","set_dosing_pump_ph_up"};

//set parameter variable
String scheduler_penyiraman,scheduler_ppm,set_dosing_pump_phDown,set_dosing_pump_phUp,set_mode_ph,set_mode_ppm,set_ph,set_ppm,set_dosing_pump_ppm,set_dosing_pump_ph_down,set_dosing_pump_ph_up = "";
byte _indexSchedulePpm = 0;
byte _indexSchedulePenyiraman = 0;
float targetPpm;
float targetPh;

bool RelayPhUp,RelayPhDown,RelayPpm;
byte RelayPhUpPin= 1;
byte RelayPhDownPin = 2;
byte RelayPpmPin = 3;

#include "stream_firebase.h"

void setup()
{
  pinMode(RelayPhUpPin,OUTPUT);
  pinMode(RelayPhDownPin,OUTPUT);
  pinMode(RelayPpmPin,OUTPUT);    
  
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  
#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 /* Rx in bytes, 512 - 16384 */, 512 /* Tx in bytes, 512 - 16384 */);
#endif
  if (!Firebase.beginMultiPathStream(streamParameter, MainPathsetParameter))
    Serial.printf("sream begin error, %s\n\n", streamParameter.errorReason().c_str());
  Firebase.setMultiPathStreamCallback(streamParameter, streamCallbackParameter, streamTimeoutCallbackParameter);    
}

void loop()
{
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Serial.print("\nSet json...");
    FirebaseJson json;
    for (size_t i = 0; i < 10; i++)
    {}
    Serial.println("ok\n");
  }
}
