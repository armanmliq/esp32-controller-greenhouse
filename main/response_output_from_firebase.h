//String childPathSetParameter[11] = {"set_dosing_pump_ppm", "set_pompa_penyiraman", "set_mode_ppm", "set_ph", "set_ppm", "set_mode_irigasi" , "set_mode_ph", "scheduler_ppm_str", "scheduler_jadwal_penyiraman", "set_dosing_pump_ph_down", "set_dosing_pump_ph_up"};

void toOutputResponse(String path, String _data) {
  if (path.indexOf("set_mode_ph") > -1)
  {
    preferences.putString("modePh", _data);
    modePhStr = _data;
    setDispIndex(0);

  } else if (path.indexOf("set_mode_ppm") > -1) {
    preferences.putString("modePpm", _data);
    modePpmStr = _data;
    setDispIndex(1);


  } else if (path.indexOf("set_ph") > -1) {
    preferences.putString("targetPh", _data);
    targetPhStr = _data;
    targetPh = targetPhStr.toFloat();
    validationTargetPhPpm();
    setDispIndex(3);


  } else if (path.indexOf("set_ppm") > -1) {
    preferences.putString("targetPpm", _data);
    targetPpmStr = _data;
    targetPpm = targetPpmStr.toFloat();
    validationTargetPhPpm();
    setDispIndex(2);

  } else if (path.indexOf("set_pompa_penyiraman") > -1) {
    if (_data == "HIDUP") {
      Serial.println("RelayPompaPenyiramanPin");
      digitalWrite(RelayPompaPenyiramanPin, 1);
    } else {
      digitalWrite(RelayPompaPenyiramanPin, 0);
    }
  } else if (path.indexOf("set_dosing_pump_ph_down") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPhDownPin, 1);
    } else {
      digitalWrite(RelayPompaPhDownPin, 0);
    }
  } else if (path.indexOf("set_dosing_pump_ph_up") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPhUpPin, 1);
    } else {
      digitalWrite(RelayPompaPhUpPin, 0);
    }
  } else if (path.indexOf("set_dosing_pump_ppm") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPpmUpPin, 1);
    } else {
      digitalWrite(RelayPompaPpmUpPin, 0);
    }
  }


  else {
    Serial.println("wrong path > " + String( path.indexOf("scheduler_jadwal_penyiraman")));
  }
}


//String schPenyiramanStr,schPpmStr,manualPhDownStr,manualPhUpStr,modePhStr,modePpmStr,targetPhStr,targetPpmStr,manualPpmUpStr = "";
