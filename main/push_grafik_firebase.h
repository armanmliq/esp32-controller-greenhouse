bool indexing = false;
FirebaseData fbdoDelete;
FirebaseData fbdoPush;

bool isQuerySettedPh        = false;
bool isQuerySettedPpm       = false;
bool isQuerySettedHumidity  = false;
bool isQuerySettedTemp      = false;

unsigned long deleteLastSecond = 2 * 24 * 3600; 
int intervalUpdateGrafik = 1;
int valDeleteAtOnce = 10;
byte savedUpGrafikState;
byte savedDeleteUpGrafikState;


void updateGrafik(String _typeSensor, String _value){
  if(WiFi.status() == WL_CONNECTED & Firebase.ready()){
    FirebaseJson json;
    json.add("ts", (uint32_t)time(nullptr));
    if(_typeSensor == "ph"){
    json.add("value", String(random(5,7)));      
    }else if(_typeSensor == "ppm"){
    json.add("value", String(random(1000,1300)));      
    }else if(_typeSensor == "humidity"){
    json.add("value", String(random(50,90)));      
    }else if(_typeSensor == "temp"){
    json.add("value", String(random(27,38)));      
    }
    
    Serial.print("Push data... "+ _typeSensor);
    String _path = "users/"+uid+"/grafik/" + _typeSensor;
    
    if (Firebase.push(fbdoPush, _path, json))
        Serial.println("ok");
    else
        Serial.println(fbdoPush.errorReason());
  }else{
    Serial.println("failed update at wifi disconn");
  }
     
}

void setQueryRules(){
  if(WiFi.status() == WL_CONNECTED & Firebase.ready()){
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
  }else{
    Serial.println("avoid set ruled at wifi disconnect");
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
    if(WiFi.status()== WL_CONNECTED & Firebase.ready()){
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
                Serial.println(fbdoDelete.errorReason());    
    }else{
      Serial.println("avoid delete nodes at wifi disconnect");
    }
}


void deleteGrafikDataLastOneDay(){
  if(hour(timeClient.getEpochTime() + offsetGmt) !=  savedDeleteUpGrafikState ){
            Serial.print("Delete history last data 2 day ... ");
            savedDeleteUpGrafikState = hour(timeClient.getEpochTime() + offsetGmt);  
            //verify unix
            if(time(nullptr) > 1618971013){
              coreDeleteGrafik();        
            }else{
              Serial.println("unix not valid");
            }
        }
  }


void updateGrafikToFirebase(){
      if(minute(timeClient.getEpochTime() + offsetGmt) -  savedUpGrafikState >= 1)
      {
          savedUpGrafikState = minute(timeClient.getEpochTime() + offsetGmt); 
          if(time(nullptr) > 1618971013){
           updateAllGrafik();          
          }else{
            Serial.println("unix not valid");
          }
      }
}

void handleGrafik(){
  Serial.println("handle..");
  updateGrafikToFirebase();
  deleteGrafikDataLastOneDay();
}
