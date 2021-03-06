#include "json.h"

#define API_KEY "AIzaSyBwu8sXyJOM-uu-NB9kfjNkIrnDkMKPoco" 
#define DATABASE_URL "https://esp32-7fe29-default-rtdb.firebaseio.com"
#define USER_EMAIL "armanmaliq92@gmail.com"
#define USER_PASSWORD "armanmaliq92@gmail.com"

unsigned long sendDataPrevMillis = 0;
String MainPathsetParameter = "/users/" + uid + "/set_parameter/";
String childPathSetParameter[11] = {"set_dosing_pump_ppm", "set_humidity","set_mode_ppm","set_ph","set_ppm", "set_mode_irigasi" ,"set_mode_ph","scheduler_ppm_str", "scheduler_jadwal_penyiraman","set_dosing_pump_ph_down","set_dosing_pump_ph_up"};

FirebaseData fbdo;
FirebaseData streamParameter;
FirebaseAuth auth;
FirebaseConfig config;

void proccessNoJson(String _data){
  Serial.println("data: "+ _data);
}


void streamCallbackParameter(MultiPathStreamData stream)
{

  size_t numChild = sizeof(childPathSetParameter) / sizeof(childPathSetParameter[0]);
  for (size_t i = 0; i < numChild; i++)
  {
    Serial.println("maxPayload:" + String(stream.maxPayloadLength()));
    Serial.println("PayloadLen:" + String(stream.payloadLength()));

    if (stream.get(childPathSetParameter[i]))
    {
      String path = stream.dataPath;
      String _data = stream.value;       
      Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : ""); 
      //cek_scheduler_ppm
//      if(path.indexOf("scheduler_ppm_str") > -1){
//         preferences.putString("schPpm", _data);
//         JsonPreprocessorSchedulePpm(_data);        
//
//        //scheduler_jadwal_penyiraman
//        }else if(path.indexOf("scheduler_jadwal_penyiraman") > -1){
//          preferences.putString("schPen", _data);
//          JsonPreprocessorSchedulePenyiraman(_data);        
//        
//        
//        //set_mode_ph
//        }else if(path.indexOf("set_mode_ph") > -1){
//          preferences.putString("modePh", _data);
//          proccessNoJson(_data);
//
//
//        //set_mode_ppm
//        }else if(path.indexOf("set_mode_ppm") > -1){
//          preferences.putString("modePpm", _data);
//          proccessNoJson(_data);
//          
//        //set_ph
//        }else if(path.indexOf("set_ph") > -1){
//          preferences.putString("targetPh", _data);
//          proccessNoJson(_data);
//
//        //set_ppm
//        }else if(path.indexOf("set_ppm") > -1){
//          preferences.putString("targetPpm", _data);
//          proccessNoJson(_data);        
//        }else{
//          Serial.println("wrong path > " +String( path.indexOf("scheduler_jadwal_penyiraman")));
//        }
          Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
          delay(1000);
    }
    
  } 
}
 void streamTimeoutCallbackParameter(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");
  if (!streamParameter.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", streamParameter.httpCode(), streamParameter.errorReason().c_str());
}

void begin_stream(){

}
