
void toOutputResponse(String path, String _data) {
  if (path.indexOf("set_mode_ph") > -1)
  {
    preferences.putString("modePh", _data);
    modePhStr = _data;
  } else if (path.indexOf("set_mode_ppm") > -1) {
    preferences.putString("modePpm", _data);
    modePpmStr = _data;

  } else if (path.indexOf("set_ph") > -1) { 
    preferences.putString("targetPh", _data);
    targetPhStr = _data;
    targetPh = targetPhStr.toFloat();


  } else if (path.indexOf("set_ppm") > -1) {
    preferences.putString("targetPpm", _data);
    targetPpmStr = _data;
    targetPpm = targetPpmStr.toFloat();

  } else if (path.indexOf("set_pompa_penyiraman") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPenyiramanPin, 1);
    } else {
      digitalWrite(RelayPompaPenyiramanPin, 0);
    }

  } else {
    Serial.println("wrong path > " + String( path.indexOf("scheduler_jadwal_penyiraman")));
  }
}


//String schPenyiramanStr,schPpmStr,manualPhDownStr,manualPhUpStr,modePhStr,modePpmStr,targetPhStr,targetPpmStr,manualPpmUpStr = "";
