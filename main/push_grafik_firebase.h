FirebaseData fbdoDelete;

bool debug = true;
bool isQuerySettedPh        = false;
bool isQuerySettedPpm       = false;
bool isQuerySettedHumidity  = false;
bool isQuerySettedTemp      = false;
bool isQuerySettedAktifitas = false;
byte savedUpGrafikState;
byte savedDeleteUpGrafikState;
String savedSendAktifitas;

void updateState(String _nodePath) {
  Serial.println(_nodePath + " OK");
  if (_nodePath == "set_pompa_ppm_up") {
    updatePpmUpStats = false;
  } else if (_nodePath == "set_pompa_penyiraman") {
    updatePenyiramanStats = false;
  } else if (_nodePath == "set_pompa_pengisian") {
    updatePengisianStats = false;
  } else if (_nodePath == "set_pompa_ph_up") {
    updatePhUpStats = false;
  } else if (_nodePath == "set_pompa_ph_down") {
    updatePhDownStats = false;
  } else if (_nodePath == "ph") {
    updatePhStats = false;
  } else if (_nodePath == "ppm") {
    updatePpmStats = false;
  } else if (_nodePath == "humidity") {
    updateHumidityStats = false;
  } else if (_nodePath == "temperatureWater") {
    updateWaterTempStats = false;
  } else if (_nodePath == "temperature") {
    updateTempRoomStats = false;
  } else if (_nodePath == "aktifitas") {
    updateAktifitasStats = false;
  }
}
void pushFirebase(String _nodePath, String _value, String _MainPath) {
  FirebaseData fbdoPush;
  FirebaseData fbdoPushParam;
  FirebaseJson json;
  String _path = "users/" + uid + "/" + _MainPath;
  json.clear();
  json.set(_nodePath, _value);
  String status = "failed";
  displayTime();
  vTaskSuspend(TaskHandle_3);
  if (internetStats & Firebase.ready()) {

    if (_nodePath.indexOf("sensor") != -1) {
      if (Firebase.updateNode(fbdoPush, _path, json)) {
        //        updateState(_nodePath);
        status = "berhasil";
      } else {
        status = "failed";
      }
    } else {
      if (Firebase.updateNode(fbdoPushParam, _path, json)) {
        //        updateState(_nodePath);
        status = "berhasil";
      } else {
        status = "failed";
      }
    }
  }
  vTaskResume(TaskHandle_3);
  Serial.println(">>>>>>updating: " + status + " " + _nodePath + " " + _value);
}

void updateGrafik(String _typeSensor, String _value) {
  FirebaseJson json;
  FirebaseData fbdoPush;
  vTaskSuspend(TaskHandle_3);
  if (internetStats & Firebase.ready()) {
    //    fbdoPush.clear();
    if (_typeSensor == "aktifitas") {
      unsigned long globalEpochLocal = globalEpoch + 3600 * 7;
      unsigned long globalEpochLocal1;
      unsigned long globalEpochLocal2;
      String _path = "users/" + uid + "/grafik/" + _typeSensor;
      if (aktifitasStr.length() > 1) {
        displayTime();

        if (aktifitasStr2.length() > 1) {
          globalEpochLocal1 = globalEpochLocal + 1;
        }
        fbdoPush.clear();
        json.clear();
        json.add("ts", String(globalEpochLocal));
        json.add("value", _value);
        fbdoPush.clear();
        if (Firebase.push(fbdoPush, _path, json)) {}
        aktifitasStr = "";
      }
      if (aktifitasStr2.length() > 1) {
        displayTime();


        globalEpochLocal2 = globalEpochLocal + 2;
        fbdoPush.clear();
        json.clear();
        json.add("ts", String(globalEpochLocal1));
        json.add("value", aktifitasStr2);
        if (Firebase.push(fbdoPush, _path, json)) {}
        aktifitasStr2 = "";
      }
      if (aktifitasStr3.length() > 1) {
        displayTime();

        fbdoPush.clear();
        json.clear();
        json.add("ts", String(globalEpochLocal2));
        json.add("value", aktifitasStr3);
        if (Firebase.push(fbdoPush, _path, json)) {}
        aktifitasStr3 = "";
      }

    } else {
      fbdoPush.clear();
      json.add("ts", (uint32_t)time(nullptr));
      json.add("value", _value);
      String _path = "users/" + uid + "/grafik/" + _typeSensor;
      Serial.println(">> up grafik: " + _path);
      if (Firebase.push(fbdoPush, _path, json)) {}
    }
  }
  vTaskResume(TaskHandle_3);
}


void setQueryRules() {
  if (internetStats & Firebase.ready()) {

    fbdoDelete.clear();
    if (!isQuerySettedAktifitas)
    {
      isQuerySettedAktifitas = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/aktifitas", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //        if (debug) Serial.println( "aktifitas setQuery ok");
      else {
        isQuerySettedAktifitas = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedPh)
    {
      isQuerySettedPh = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/ph", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //        if (debug) Serial.println( "ph setQuery ok");
      else {
        isQuerySettedPh = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedPpm)
    {
      isQuerySettedPpm = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/ppm", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //        if (debug) Serial.println( "ppm setQuery ok");
      else {
        isQuerySettedPpm = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedHumidity)
    {
      isQuerySettedHumidity = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/humidity", "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //if (debug) Serial.println( "humidity setQuery ok");
      else {
        isQuerySettedHumidity = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedTemp)
    {
      isQuerySettedTemp = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/temp" , "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //        if (debug) Serial.println( + "temp setQuery ok");
      else {
        isQuerySettedTemp = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedTemp)
    {
      isQuerySettedTemp = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/waterTemp" , "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //        if (debug) Serial.println( + "waterTemp setQuery ok");
      else {
        isQuerySettedTemp = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
    if (!isQuerySettedTemp)
    {
      isQuerySettedTemp = true;
      //      if (debug) Serial.print("Set query index in database rules..  ");
      if (Firebase.setQueryIndex(fbdoDelete, "users/" + uid + "/grafik/statusPompaPenyiraman" , "ts", "Kg7ULKnG0RSjFugCV96QD2LMNzkEydiTPgpuqAWW")) {

      }
      //        if (debug) Serial.println( + "statusPompaPenyiraman setQuery ok");
      else {
        isQuerySettedTemp = false;
      }

      //      if (debug) Serial.println(fbdoDelete.errorReason());
    }
  } else {
    //    if (debug) Serial.println("avoid set ruled at wifi disconnect");
  }
}

void updateAllGrafik() {
  setQueryRules();
  updateGrafik("ph", String(sensPh, 1));
  updateGrafik("ppm", String(sensPpm, 0));
  updateGrafik("humidity", String(sensHumidity, 1));
  updateGrafik("temp", String(sensTempRoom, 1));
  updateGrafik("waterTemp", String(sensTempWater, 1));
}
void coreDeleteGrafik() {
  //  if (internetStats & Firebase.ready()) {
  //    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/ph", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
  //      if (debug) Serial.println("del ph ok");
  //      else if (debug) Serial.println(fbdoDelete.errorReason());
  //
  //    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/ppm", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
  //      if (debug) Serial.println("del ppm ok");
  //      else if (debug) Serial.println(fbdoDelete.errorReason());
  //
  //    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/humidity", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
  //      if (debug) Serial.println("del humidity ok");
  //      else if (debug) Serial.println(fbdoDelete.errorReason());
  //
  //    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/temp", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
  //      if (debug) Serial.println("del ppm ok");
  //      else if (debug) Serial.println(fbdoDelete.errorReason());
  //
  //    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/humidity", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
  //      if (debug) Serial.println("del humidity ok");
  //      else if (debug) Serial.println(fbdoDelete.errorReason());
  //    if (Firebase.deleteNodesByTimestamp(fbdoDelete, "users/" + uid + "/grafik/aktifitas", "ts", deleteGrafikItemAtOnce , deleteGrafikSecondExceedFrom ))
  //      if (debug) Serial.println("del aktifitas ok");
  //      else if (debug) Serial.println(fbdoDelete.errorReason());
  //  } else {
  //    if (debug) Serial.println("avoid delete nodes at wifi disconnect");
  //  }
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
  if (globalMinute -  savedUpGrafikState >= 2 & globalMinute % 2 == 0)
  {
    savedUpGrafikState = globalMinute;
    Serial.println(">>>>>>>>>update grafik<<<<<<");
    updateAllGrafik();
  }
}

void handleGrafik() {
  if (WiFi.status() == WL_CONNECTED & internetStats) {
    updateGrafikToFirebase();
  }
  //  deleteGrafikDataLastOneDay();
}
