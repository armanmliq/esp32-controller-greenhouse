void toOutputResponse(String path, String _data) {
  if (path.indexOf("set_mode_ph") > -1)
  {
    preferences.putString("modePh", _data);
    parsingModePh(_data);
    setDispIndex(0);

  } else if (path.indexOf("set_mode_ppm") > -1) {
    preferences.putString("modePpm", _data);
    parsingModePpm(_data);
    setDispIndex(1);


  } else if (path.indexOf("set_ph") > -1) {
    preferences.putString("targetPh", _data);
    parsingTargetPh(_data);
    setDispIndex(3);


  } else if (path.indexOf("set_ppm") > -1) {
    preferences.putString("targetPpm", _data);
    parsingTargetPpm(_data);
    setDispIndex(2);

  } else if (path.indexOf("set_pompa_penyiraman") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPenyiramanPin, 1);
    } else {
      digitalWrite(RelayPompaPenyiramanPin, 0);
    }
  } else if (path.indexOf("set_pompa_ph_down") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPhDownPin, 1);
    } else {
      digitalWrite(RelayPompaPhDownPin, 0);
    }
  } else if (path.indexOf("set_pompa_ph_up") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPhUpPin, 1);
    } else {
      digitalWrite(RelayPompaPhUpPin, 0);
    }
  } else if (path.indexOf("set_pompa_ppm_up") > -1) {
    if (_data == "HIDUP") {
      Serial.println("HIDUP RelayPompaPpm");
      digitalWrite(RelayPompaPpmUpPin, 1);
    } else {
      digitalWrite(RelayPompaPpmUpPin, 0);
    }
  } else if (path.indexOf("set_pompa_pengisian") > -1) {
    if (_data == "HIDUP") {
      digitalWrite(RelayPompaPengisianPin, 1);
    } else {
      digitalWrite(RelayPompaPengisianPin, 0);
    }
  } else if (path.indexOf("set_batas_margin_ph") > -1) {
    preferences.putString("batasMarginPh", _data);
    parsingBatasMarginPh(_data);
  } else if (path.indexOf("set_interval_on_ph") > -1) {
    preferences.putString("intervalOnPh", _data);
    parsingIntervalOnPh(_data);
  } else if (path.indexOf("set_interval_off_ph") > -1) {
    preferences.putString("intervalOffPh", _data);
    parsingIntervalOffPh(_data);
  } else if (path.indexOf("set_interval_on_ppm") > -1) {
    preferences.putString("intervalOnPpm", _data);
    parsingIntervalOnPpm(_data);
  } else if (path.indexOf("set_interval_off_ppm") > -1) {
    preferences.putString("intervalOffPpm", _data);
    parsingIntervalOffPpm(_data);
  } else if (path.indexOf("set_interval_limit") > -1) {
    preferences.putString("intervalLimit", _data);
    parsingIntervalLimit(_data);
  }
  else {
    Serial.println("wrong path > " + String( path.indexOf("scheduler_jadwal_penyiraman")));
  }
}
