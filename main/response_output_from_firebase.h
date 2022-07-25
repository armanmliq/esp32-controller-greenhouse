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
    setDispIndex(2);

  } else if (path.indexOf("set_sprayer") > -1) {
    preferences.putString("set_sprayer", _data);
    parsingSprayer(_data);
    setDispIndex(4);
  } else if (path.indexOf("set_ppm") > -1) {
    preferences.putString("targetPpm", _data);
    parsingTargetPpm(_data);
    setDispIndex(3);
  } else if (path.indexOf("set_pompa_penyiraman") > -1) {
    preferences.putString("setPenyiraman", _data);
    parsingPompaPenyiraman(_data);
  } else if (path.indexOf("set_pompa_ph_down") > -1) {
    parsingPompaPhDown(_data);
  } else if (path.indexOf("set_pompa_ph_up") > -1) {
    parsingPompaPhUp(_data);
  } else if (path.indexOf("set_pompa_ppm_up") > -1) {
    parsingPompaPpmUp(_data);
  } else if (path.indexOf("set_pompa_ppm_down") > -1) {
    parsingPompaPpmDown(_data);
  }  else if (path.indexOf("set_pompa_pengisian") > -1) {
    preferences.putString("setPengisian", _data);
    parsingPompaPengisian(_data);
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
  } else if (path.indexOf("set_margin_ppm") > -1) {
    preferences.putString("batasMarginPpm", _data);
    parsingBatasMarginPpm(_data);
  } else if (path.indexOf("set_interval_cek_ph") > -1) {
    preferences.putString("intervalPh", _data);
    parsingIntervalCekPh(_data);
  } else if (path.indexOf("set_interval_cek_ppm") > -1) {
    preferences.putString("intervalPpm", _data);
    parsingIntervalCekPpm(_data);
  } else if (path.indexOf("set_interval_report") > -1) {
    preferences.putString("intervalReport", _data);
    parsingIntervalReport(_data);
  } else if (path.indexOf("set_interval_detect_all") > -1) {
    preferences.putString("intervalDetect", _data);
    parsingIntervalDetectAll(_data);
  } else if (path.indexOf("set_interval_notif_oto") > -1) {
    preferences.putString("intervalNotif", _data);
    parsingIntervalNotif(_data);
  } else if (path.indexOf("set_atur_ph_ppm") > -1) {
    parsingAturPhPpm(_data);
  }
  else {
    Serial.println("wrong path X " + String( path.indexOf("scheduler_jadwal_penyiraman")));
  }
}
void allRelayOff() {
  digitalWrite(RelayPompaPengisianPin, !LOW);
  digitalWrite(RelayPompaPenyiramanPin, !LOW);
  digitalWrite(RelayValvePenyiramanPin, !LOW);
  digitalWrite(RelayPompaPhUpPin, !LOW);
  digitalWrite(RelayPompaPhDownPin, !LOW);
  digitalWrite(RelayPompaPpmUpPin, !LOW);
  digitalWrite(RelaySprayerPin, !LOW);
  digitalWrite(RelayValvePenyiramanPin, !LOW);
}
