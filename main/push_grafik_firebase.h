FirebaseData fbdoDelete;
unsigned int deleteLast2day = 172800; //second last 2 day
byte savedDeleteUpGrafikState;
byte savedUpGrafikState;
bool isQuerySetted = false;
int intervalUpGrafik = 20; // in minute 
String mainPathGrafik = "users/" + uid + "/grafik/";
void updateGrafik(String _typeSensor, String _value){
FirebaseJson json;
         if (!isQuerySetted)
        {
            isQuerySetted = true;
            Serial.print("Set query index in database rules... "); 
            if (Firebase.setQueryIndex(fbdoDelete, "users/", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
                Serial.println("ph>ok");
            else
                Serial.println(fbdoDelete.errorReason()); 
        }

            json.add("ts", (uint32_t)time(nullptr));
            json.add("value", _value);

            Serial.print("Push data... ");
            String _path = mainPathGrafik + _typeSensor;
            
            if (Firebase.push(fbdoDelete, _path, json))
                Serial.println("ok");
            else
                isQuerySetted = false;
                Serial.println(fbdoDelete.errorReason()); 
}

void coreUpdateAllGrafik(){
  updateGrafik("ph",String(sensPh));
  updateGrafik("ppm",String(sensPpm));
  updateGrafik("humidity",String(sensHumidity));
  updateGrafik("temp",String(sensTemperature));
}



void coreDeleteUpGrafik(){
              if (Firebase.deleteNodesByTimestamp(fbdoDelete, mainPathGrafik + "ph/", "ts", 288 , 10 ))
                Serial.println("ok");
            else
                Serial.println(fbdoDelete.errorReason());    

            if (Firebase.deleteNodesByTimestamp(fbdoDelete, mainPathGrafik + "ppm/", "ts", 288 , 10 ))
                Serial.println("ok");
            else
                Serial.println(fbdoDelete.errorReason());    

            if (Firebase.deleteNodesByTimestamp(fbdoDelete, mainPathGrafik + "humidity/", "ts", 288 , 10 ))
                Serial.println("ok");
            else
                Serial.println(fbdoDelete.errorReason());    

            if (Firebase.deleteNodesByTimestamp(fbdoDelete, mainPathGrafik+ "temp/", "ts", 288 , 10 ))
                Serial.println("ok");
            else
                Serial.println(fbdoDelete.errorReason());    
}



void deleteGrafikDataLast2Day(){
  if(day(timeClient.getEpochTime() + offsetGmt) !=  savedDeleteUpGrafikState & isQuerySetted){
            Serial.print("Delete history data 1day second... ");
            savedDeleteUpGrafikState = day(timeClient.getEpochTime() + offsetGmt);
            coreDeleteUpGrafik(); 
  }
}


void updateGrafikToFirebase(){
      if(minute(timeClient.getEpochTime() + offsetGmt) -  savedUpGrafikState >= intervalUpGrafik){
         savedUpGrafikState = minute(timeClient.getEpochTime() + offsetGmt);
         coreUpdateAllGrafik();
      }
}

void handleGrafik(){
  updateGrafikToFirebase();
  deleteGrafikDataLast2Day();
}
