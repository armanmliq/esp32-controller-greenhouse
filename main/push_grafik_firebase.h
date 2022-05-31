bool indexing = false;
FirebaseData fbdoDelete;

bool isQuerySettedPh        = false;
bool isQuerySettedPpm       = false;
bool isQuerySettedHumidity  = false;
bool isQuerySettedTemp      = false;

unsigned long deleteLastSecond = 2 * 24 * 3600; //second in 2 day
int intervalUpdateGrafik = 10;
int valDeleteAtOnce = (60 / intervalUpdateGrafik) * 24;
byte savedUpGrafikState;
byte savedDeleteUpGrafikState;


void updateGrafik(String _typeSensor, String _value){
    FirebaseJson json;
    json.add("ts", (uint32_t)time(nullptr));
    json.add("_value", _value);

    Serial.print("Push data... "+ _typeSensor);
    String _path = "users/"+uid+"/grafik/" + _typeSensor;
    
    if (Firebase.push(fbdoDelete, _path, json))
        Serial.println("ok");
    else
        Serial.println(fbdoDelete.errorReason()); 
}
void setQueryRules(){
     if (!isQuerySettedPh)
  {
      isQuerySettedPh = true;
      Serial.print("Set query index in database rules..  "); 
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid+ "/grafik/ph", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
          Serial.println( "ph setQuery ok");
      else
          isQuerySettedPh = false;
          Serial.println(fbdoDelete.errorReason()); 
  }

     if (!isQuerySettedPpm)
  {
      isQuerySettedPpm = true;
      Serial.print("Set query index in database rules..  "); 
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid+ "/grafik/ppm", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
          Serial.println( "ppm setQuery ok");
      else
          isQuerySettedPpm = false;
          Serial.println(fbdoDelete.errorReason()); 
  }
  
       if (!isQuerySettedHumidity)
  {
      isQuerySettedHumidity = true;
      Serial.print("Set query index in database rules..  "); 
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid+ "/grafik/humidity", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
          Serial.println( "humidity setQuery ok");
      else
          isQuerySettedHumidity = false;
          Serial.println(fbdoDelete.errorReason()); 
  }

       if (!isQuerySettedTemp)
  {
      isQuerySettedTemp = true;
      Serial.print("Set query index in database rules..  "); 
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid+ "/grafik/temp" ,"ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
          Serial.println( + "temp setQuery ok");
      else
          isQuerySettedTemp = false;
          Serial.println(fbdoDelete.errorReason()); 
  }
  
}

void updateAllGrafik(){
  setQueryRules();
  updateGrafik("ph",String(sensPh));
  updateGrafik("ppm",String(sensPpm));
  updateGrafik("humidity",String(sensHumidity));
  updateGrafik("temp",String(sensTemperature));
}


void coreDeleteGrafik(){
              if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/ph", "ts", valDeleteAtOnce , deleteLastSecond ))
                Serial.println("del ph ok");
            else
                Serial.println(fbdoDelete.errorReason());    

            if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid+ "/grafik/ppm", "ts", valDeleteAtOnce , deleteLastSecond ))
                Serial.println("del ppm ok");
            else
                Serial.println(fbdoDelete.errorReason());    

            if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid+ "/grafik/humidity", "ts", valDeleteAtOnce , deleteLastSecond ))
                Serial.println("del humidity ok");
            else
                Serial.println(fbdoDelete.errorReason());    

            if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/temp", "ts", valDeleteAtOnce , deleteLastSecond ))
                Serial.println("del ppm ok");
            else
                Serial.println(fbdoDelete.errorReason());    
}


void deleteGrafikDataLastOneDay(){
  if(day(timeClient.getEpochTime() + offsetGmt) !=  savedDeleteUpGrafikState ){
            Serial.print("Delete history data 1 day second... ");
            savedDeleteUpGrafikState = day(timeClient.getEpochTime() + offsetGmt); 

            //verify unix
            if(time(nullptr) > 1618971013){
              coreDeleteGrafik();        
            }else{
              Serial.println("unix not valid");
            }
        }
  }


void updateGrafikToFirebase(){
      if(minute(timeClient.getEpochTime() + offsetGmt) -  savedUpGrafikState >= intervalUpdateGrafik){
        savedUpGrafikState = minute(timeClient.getEpochTime() + offsetGmt);
       
        if(time(nullptr) > 1618971013){
          
         updateAllGrafik();          
        }else{
          Serial.println("unix not valid");
        }

      }
}

void handleGrafik(){
  updateGrafikToFirebase();
  deleteGrafikDataLastOneDay();
}
