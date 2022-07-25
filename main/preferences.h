#include <Preferences.h>
Preferences preferences;

unsigned long mDetectFirstBootPengisian, mDetectFirstBootPenyiraman, mDetectFirstBootSprayer;
bool isFirstBootPenyiraman, isFirstBootPengisian, isFirstBootSprayer;
String initialSprayer;

void parsingAturPhPpm(String strValue) {
  if (!isPpmMixingBegin & modePpmStr == "MANUAL") {
    Serial.println("atur ppm aktif");
    isPpmMixingBegin = true;
    limSendNotifModePpm = false;
    limSendCalculatePpmInfo = false;
  }
  if (!isPhMixingBegin & modePhStr == "MANUAL") {
    Serial.println("atur ppm aktif");
    isPhMixingBegin = true;
    limSendNotifModePh = false;
    limSendCalculatePhInfo = false;
  }
}

void parsingInitialPenyiraman(String strValue) {
  initialPenyiraman = strValue;
}


void parsingInitialPengisian(String strValue) {
  initialPengisian = strValue;
}

void parsingInitialSprayer(String strValue) {
  initialSprayer = strValue;
}

void parsingSprayer(String strValue) {
  if (!isFirstBootSprayer)
  {
    isFirstBootSprayer = true;
    mDetectFirstBootSprayer = millis();
  }
  if (strValue == "HIDUP") {
    bool isFirstBoot = millis() - mDetectFirstBootSprayer < 2500 ? true : false;
    Serial.print(">>isFirstBootSprayer: ");
    Serial.println(isFirstBoot);
    savedStatsSprayer = 1;

    //pembacaan pertama
    if (isFirstBoot) {
      parsingInitialSprayer(preferences.getString("initSprayer"));
      if (initialSprayer == "HIDUP") {
        digitalWrite(RelaySprayerPin, !HIGH);
        preferences.putString("initSprayer", "HIDUP");
      }
    } else {
      preferences.putString("initSprayer", "HIDUP");
      digitalWrite(RelaySprayerPin, !HIGH);
    }
  } else {
    preferences.putString("initSprayer", "MATI");
    savedStatsSprayer = 0;
    digitalWrite(RelaySprayerPin, !LOW);
  }
}

void parsingPompaPengisian(String strValue) {
  if (!isFirstBootPengisian)
  {
    isFirstBootPengisian = true;
    mDetectFirstBootPengisian = millis();
  }
  if (strValue == "HIDUP") {
    bool isFirstBoot = millis() - mDetectFirstBootPengisian < 2500 ? true : false;
    Serial.print(">>isFirstBootPengisian: ");
    Serial.println(isFirstBoot);
    savedStatsPengisian = 1;

    //pembacaan pertama
    if (isFirstBoot) {
      parsingInitialPengisian(preferences.getString("initPengisian"));
      if (initialPengisian == "HIDUP") {
        digitalWrite(RelayPompaPengisianPin, !HIGH);
      }
    } else {
      preferences.putString("initPengisian", "HIDUP");
      digitalWrite(RelayPompaPengisianPin, !HIGH);
    }
  } else {
    preferences.putString("initPengisian", "MATI");
    savedStatsPengisian = 0;
    digitalWrite(RelayPompaPengisianPin, !LOW);
  }
}

void parsingPompaPenyiraman(String strValue) {
  if (!isFirstBootPenyiraman)
  {
    isFirstBootPenyiraman = true;
    mDetectFirstBootPenyiraman = millis();
  }
  if (strValue == "HIDUP") {
    bool isFirstBoot = millis() - mDetectFirstBootPenyiraman < 2500 ? true : false;
    Serial.print(">>isFirstBootPenyiraman: ");
    Serial.println(isFirstBoot);
    savedStatsPenyiraman = 1;

    //pembacaan pertama
    if (isFirstBoot) {
      parsingInitialPenyiraman(preferences.getString("initSiram"));
      if (initialPenyiraman == "HIDUP") {
        digitalWrite(RelayPompaPenyiramanPin, !HIGH);
        digitalWrite(RelayValvePenyiramanPin, !HIGH);
      }
    } else {
      preferences.putString("initSiram", "HIDUP");
      digitalWrite(RelayPompaPenyiramanPin, !HIGH);
      digitalWrite(RelayValvePenyiramanPin, !HIGH);
    }
  } else {
    preferences.putString("initSiram", "MATI");
    savedStatsPenyiraman = 0;
    digitalWrite(RelayPompaPenyiramanPin, !LOW);
    digitalWrite(RelayValvePenyiramanPin, !LOW);
  }
}

void parsingTargetPpm(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < maksPpm) {
    targetPpm = strValue.toFloat();
    savedStatsTargetPpm = targetPpm;
  }
}
void parsingTargetPh(String strValue) {
  if (strValue.toFloat() >= 2 & strValue.toFloat() < 14) {
    targetPh = strValue.toFloat();
  }
}
void parsingModePh(String strValue) {
  if (strValue == "MANUAL" || strValue == "OTOMATIS") {
    if (modePhStr != strValue) {
      digitalWrite(RelayPompaPhUpPin, !LOW);
      digitalWrite(RelayPompaPhDownPin, !LOW);
      modePhStr = strValue;
    }
  }
}
void parsingModePpm(String strValue) {
  if (strValue == "MANUAL" || strValue == "OTOMATIS") {
    digitalWrite(RelayPompaPpmUpPin, !LOW);
    modePpmStr = strValue;
  }
}
void parsingIntervalOnPh(String strValue) {
  if (strValue.toInt() > 999 & strValue.toInt() < maksIntervalOn) {
    intervalOnPh = strValue.toInt();
  }
}
void parsingIntervalOffPh(String strValue) {
  if (strValue.toInt() > 999 & strValue.toInt() < maksIntervalOff) {
    intervalOffPh = strValue.toInt();
  }
}

void parsingIntervalOnPpm(String strValue) {
  if (strValue.toInt() > 999 & strValue.toInt() < maksIntervalOn) {
    intervalOnPpm = strValue.toInt();
  }
}
void parsingIntervalOffPpm(String strValue) {
  if (strValue.toInt() > 999 & strValue.toInt() < maksIntervalOff) {
    intervalOffPpm = strValue.toInt();
  }
}
void parsingIntervalLimit(String strValue) {
  if (strValue.toInt() > 6000 & strValue.toInt() < intervalLimit) {
    intervalLimit = strValue.toInt();
  }
}
void parsingBatasMarginPh(String strValue) {
  if (strValue.toFloat() > 0.1 & strValue.toFloat() < 4) {
    batasMarginPh = strValue.toFloat();
  }
}

void parsingBatasMarginPpm(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < 1000) {
    batasMarginPpm = strValue.toFloat();
  }
}

void parsingIntervalCekPpm(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < 60 * 60 * 5) { // min 5 jam
    intervalCekPpm = strValue.toFloat();
  }
}

void parsingIntervalCekPh(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < 60 * 60 * 5) { // min 5 jam
    intervalCekPh = strValue.toFloat();
  }
}

void parsingIntervalReport(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < 60 * 60 * 24) { // min 24 jam
    intervalSendReport = strValue.toFloat();
  }
}
void parsingIntervalDetectAll(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < 60 * 60 * 5) { // min 5 jam
    intervalDetectAll = strValue.toFloat();
  }
}

void parsingIntervalNotif(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < 60 * 60 * 5) { // min 5 jam
    intervalNotif = strValue.toFloat();
    intervalNotifOtomatisPh = intervalNotif * 1000;
    intervalNotifOtomatisPpm = intervalNotif * 1000;
  }
}
void parsingPompaPhUp(String strValue) {
  if (modePhStr == "MANUAL") {
    if (strValue == "HIDUP") {
      digitalWrite(RelayPompaPhUpPin, !HIGH);
    } else {
      digitalWrite(RelayPompaPhUpPin, !LOW);
    }
  }
}
void parsingPompaPpmUp(String strValue) {
  if (modePpmStr == "MANUAL") {
    if (strValue == "HIDUP") {
      digitalWrite(RelayPompaPpmUpPin, !HIGH);
    } else {
      digitalWrite(RelayPompaPpmUpPin, !LOW);
    }
  }
}

void parsingPompaPhDown(String strValue) {
  if (modePhStr == "MANUAL") {
    if (strValue == "HIDUP") {
      digitalWrite(RelayPompaPhDownPin, !HIGH);
    } else {
      digitalWrite(RelayPompaPhDownPin, !LOW);
    }
  }
}

void parsingPompaPpmDown(String strValue) {
  if (modePpmStr == "MANUAL") {
    if (strValue == "HIDUP") {
      digitalWrite(RelayPompaPpmDownPin, !HIGH);
    } else {
      digitalWrite(RelayPompaPpmDownPin, !LOW);
    }
  }
}

void parsingInternalData() {

  schPpmStr = preferences.getString("schPpm");
  schPenyiramanStr = preferences.getString("schPen");

  String _modePhStr = preferences.getString("modePh");
  String _modePpmStr = preferences.getString("modePpm");
  String _targetPhStr = preferences.getString("targetPh");
  String _targetPpmStr = preferences.getString("targetPpm");
  String _intervalOnPhStr = preferences.getString("intervalOnPh");
  String _intervalOffPhStr = preferences.getString("intervalOffPh");
  String _intervalOnPpmStr = preferences.getString("intervalOnPpm");
  String _intervalOffPpmStr = preferences.getString("intervalOffPpm");
  String _intervalLimitStr = preferences.getString("intervalLimit");
  String _batasMarginPhStr = preferences.getString("batasMarginPh");
  String _sprayerStr = preferences.getString("set_sprayer");
  String _setPenyiramanStr = preferences.getString("setPenyiraman");
  String _setIntervalCekPh = preferences.getString("intervalPh");
  String _setIntervalCekPpm = preferences.getString("intervalPpm");
  String _setIntervalSendReport = preferences.getString("intervalReport");
  String _setIntervalNotif = preferences.getString("intervalNotif");
  String _setIntervalDetectAll = preferences.getString("intervalDetect");
  String _batasMarginPpmStr = preferences.getString("batasMarginPpm");
  String _initialPenyiraman = preferences.getString("initSiram");
  String _initialPengisian = preferences.getString("initPengisian");
  String _initialSprayer = preferences.getString("initSprayer");

  parsingInitialSprayer(_initialSprayer);
  parsingInitialPenyiraman(_initialPenyiraman);
  parsingInitialPengisian(_initialPengisian);
  parsingIntervalNotif(_setIntervalNotif);
  parsingBatasMarginPpm(_batasMarginPpmStr);
  parsingBatasMarginPpm(_batasMarginPpmStr);
  parsingIntervalDetectAll(_setIntervalDetectAll);
  parsingIntervalReport(_setIntervalSendReport);
  parsingIntervalCekPh(_setIntervalCekPh);
  parsingIntervalCekPpm(_setIntervalCekPpm);
  parsingInitialSprayer(_sprayerStr);
  parsingTargetPpm(_targetPpmStr);
  parsingTargetPh(_targetPhStr);
  parsingModePh(_modePhStr);
  parsingModePpm(_modePpmStr);
  parsingIntervalOnPh(_intervalOnPhStr);
  parsingIntervalOffPh(_intervalOffPhStr);
  parsingIntervalOnPpm(_intervalOnPpmStr);
  parsingIntervalOffPpm(_intervalOffPpmStr);
  parsingIntervalLimit(_intervalLimitStr);
  parsingBatasMarginPh(_batasMarginPhStr);
  parsingPompaPenyiraman(_setPenyiramanStr );
  JsonPreprocessorSchedulePpm(schPpmStr);
  parsingIntervalNotif(_setIntervalNotif);
  JsonPreprocessorSchedulePenyiraman(schPenyiramanStr);

  Serial.println("_modePhStr: " + _modePhStr);
  Serial.println("_modePpmStr: " + _modePpmStr);
  Serial.println("_targetPhStr: " + _targetPhStr);
  Serial.println("_targetPpmStr: " + _targetPpmStr);
  Serial.println("_intervalOnPhStr: " + String(_intervalOnPhStr));
  Serial.println("_intervalOffPhStr: " + String(_intervalOffPhStr));
  Serial.println("_intervalOnPpmStr: " + String(_intervalOnPpmStr));
  Serial.println("_intervalOffPpmStr: " + String(_intervalOffPpmStr));
  Serial.println("_intervalLimitStr: " + String(_intervalLimitStr));
  Serial.println("_batasMarginPhStr: " + String(_batasMarginPhStr));
  Serial.println("_sprayerStr: " + String(_sprayerStr));
  Serial.println("_setPenyiramanStr: " + String(_setPenyiramanStr));
  Serial.println("_setIntervalCekPh: " + _setIntervalCekPh);
  Serial.println("_setIntervalCekPpm: " + _setIntervalCekPpm);
  Serial.println("_setIntervalSendReport: " + _setIntervalSendReport);
  Serial.println("_setIntervalNotif: " + String(intervalNotif));
  Serial.println("_setIntervalDetectAll: " + String(_setIntervalDetectAll));
  Serial.println("_batasMarginPpmStr: " + String(_batasMarginPpmStr));
  Serial.println("_initialPenyiraman: " + String(_initialPenyiraman));
  Serial.println("_initialPengisian: " + _initialPengisian);
  Serial.println("_initialSprayer: " + _initialSprayer);
  Serial.println("intervalNotif: " + _setIntervalNotif);
  Serial.println("delay 5 sec");
}
