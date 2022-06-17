FirebaseData fbdoDelete;
FirebaseData fbdoPush;
FirebaseData fbdoPushParam;
FirebaseJson json;
FirebaseJson jsonParam;

bool debug = false;


bool isQuerySettedPh        = false;
bool isQuerySettedPpm       = false;
bool isQuerySettedHumidity  = false;
bool isQuerySettedTemp      = false;

byte savedUpGrafikState;
byte savedDeleteUpGrafikState;

void pushFirebase(String _nodePath, String _value,String _MainPath) {
  String _path = "users/" + uid + "/"+_MainPath;
  json.set(_nodePath, _value);
  Serial.println("updating >>> " + _nodePath + " " + String(_value));
  if (WiFi.status() == WL_CONNECTED & Firebase.ready()) {
    Firebase.updateNode(fbdoPush, _path, json);
  }
}
void updateGrafik(String _typeSensor, String _value) {
  if (WiFi.status() == WL_CONNECTED & Firebase.ready()) {
    FirebaseJson json;
    json.add("ts", (uint32_t)time(nullptr));
    json.add("value", _value);

    if (debug) Serial.print("Push data... " + _typeSensor);
    String _path = "users/" + uid + "/grafik/" + _typeSensor;

    if (Firebase.push(fbdoPush, _path, json))
      if (debug) Serial.println("ok");
      else if (debug) Serial.println(fbdoPush.errorReason());
  } else {
    if (debug) Serial.println("failed update at wifi disconn");
  }
}

void setQueryRules() {
  if (WiFi.status() == WL_CONNECTED & Firebase.ready()) {
    if (!isQuerySettedPh)
    {
      isQuerySettedPh = true;
      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/ph", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
        if (debug) Serial.println( "ph setQuery ok");
        else
          isQuerySettedPh = false;
      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedPpm)
    {
      isQuerySettedPpm = true;
      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/ppm", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
        if (debug) Serial.println( "ppm setQuery ok");
        else
          isQuerySettedPpm = false;
      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedHumidity)
    {
      isQuerySettedHumidity = true;
      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/humidity", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
        if (debug) Serial.println( "humidity setQuery ok");
        else
          isQuerySettedHumidity = false;
      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedTemp)
    {
      isQuerySettedTemp = true;
      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/temp" , "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
        if (debug) Serial.println( + "temp setQuery ok");
        else
          isQuerySettedTemp = false;
      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedTemp)
    {
      isQuerySettedTemp = true;
      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/waterTemp" , "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
        if (debug) Serial.println( + "waterTemp setQuery ok");
        else
          isQuerySettedTemp = false;
      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedTemp)
    {
      isQuerySettedTemp = true;
      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/statusPompaPenyiraman" , "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW"))
        if (debug) Serial.println( + "statusPompaPenyiraman setQuery ok");
        else
          isQuerySettedTemp = false;
      if (debug) Serial.println(fbdoDelete.errorReason());
    }
  } else {
    if (debug) Serial.println("avoid set ruled at wifi disconnect");
  }
}

void updateAllGrafik() {
  setQueryRules();
  updateGrafik("ph", String(sensPh));
  updateGrafik("ppm", String(sensPpm));
  updateGrafik("humidity", String(sensHumidity));
  updateGrafik("temp", String(sensTempRoom));
  updateGrafik("waterTemp", String(sensTempWater));
}
void coreDeleteGrafik() {
  if (WiFi.status() == WL_CONNECTED & Firebase.ready()) {
    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/ph", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
      if (debug) Serial.println("del ph ok");
      else if (debug) Serial.println(fbdoDelete.errorReason());

    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/ppm", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
      if (debug) Serial.println("del ppm ok");
      else if (debug) Serial.println(fbdoDelete.errorReason());

    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/humidity", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
      if (debug) Serial.println("del humidity ok");
      else if (debug) Serial.println(fbdoDelete.errorReason());

    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/temp", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
      if (debug) Serial.println("del ppm ok");
      else if (debug) Serial.println(fbdoDelete.errorReason());

    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/humidity", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
      if (debug) Serial.println("del humidity ok");
      else if (debug) Serial.println(fbdoDelete.errorReason());
  } else {
    if (debug) Serial.println("avoid delete nodes at wifi disconnect");
  }
}
void deleteGrafikDataLastOneDay() {
  if (globalHour !=  savedDeleteUpGrafikState ) {
    if (debug) Serial.print("Delete history last data 2 day ... ");
    savedDeleteUpGrafikState = globalHour;
    //verify unix
    if (time(nullptr) > 1618971013) {
      coreDeleteGrafik();
    } else {
      if (debug) Serial.println("unix not valid");
    }
  }
}
void updateGrafikToFirebase() {
  if (globalMinute -  savedUpGrafikState >= 1)
  {
    savedUpGrafikState = globalMinute;
    if (time(nullptr) > 1618971013) {
      updateAllGrafik();
    } else {
      if (debug) Serial.println("unix not valid");
    }
  }
}

void handleGrafik() {
  if (debug) Serial.println("handle..");
  updateGrafikToFirebase();
  deleteGrafikDataLastOneDay();
}
