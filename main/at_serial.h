bool _state;
void serial() {
  if (Serial.available() > 0)
  {
    char dat = Serial.read();
    if (dat == 'e') {
      updateAllGrafik();
    }

    if (dat == 'i') {
      setup_firebase();
    }

    if (dat == 'o') {
      begin_stream();
    }

    if (dat == 'm') {
      digitalWrite(RelayPompaPpmDownPin, !digitalRead(RelayPompaPpmDownPin));
      Serial.println(digitalRead(RelayPompaPpmDownPin));
    }

    //switch float
    if (dat == 'p') {
      sensFloat = !sensFloat;
    }

    //dummy nilai ph
    if (dat == 'q') {
      modeDummyPh = !modeDummyPh;
      dummyPh = Serial.parseFloat();
      Serial.println("DUMMY PH :" + String(dummyPh));
    }

    //dummy nilai ppm
    if (dat == 'r') {
      modeDummyPpm = !modeDummyPpm;
      dummyPpm = Serial.parseFloat();
      Serial.println("DUMMY PPM :" + String(dummyPpm));
    }

    //aktifkan pengecekan ph
    if (dat == 's') {
      enableCheckingPh();
    }

    //aktifkan pengecekan ppm
    if (dat == 't') {
      enableCheckingPpm();
    }

    //tes preferences write
    if (dat == 'u') {
      _state = !_state;
      String _stateStr = _state ? "HIDUP" : "MATI";
      Serial.println("saving.. " + _stateStr);
      preferences.putString("initPengisian", _stateStr);
    }

    //tes preferences write
    if (dat == 'v') {
      String _stateStr = preferences.getString("initPengisian");
      Serial.println("read :" + _stateStr);
    }
    //tes preferences write
    if (dat == 'w') {
      Serial.println("update ntp");
      updateNtp();
    }
    //init rtc
    if (dat == 'x') {
      Serial.println("try read rtc");
      isRtcConnected = true;
      if (!rtc.begin()) {
        isRtcConnected = false;
        Serial.println("Couldn't find RTC");
      }
    }
    if (dat == 'z') {
      Serial.println("restart");
      ESP.restart();
    }
  }
}
