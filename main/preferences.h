#include <Preferences.h>
Preferences preferences;
void parsingTargetPpm(String strValue) {
  if (strValue.toFloat() > 0 & strValue.toFloat() < maksPpm) {
    targetPpm = strValue.toFloat();
  }
}
void parsingTargetPh(String strValue) {
  if (strValue.toFloat() >= 2 & strValue.toFloat() < 14) {
    targetPh = strValue.toFloat();
  }
}
void parsingModePh(String strValue) {
  if (strValue == "MANUAL" || strValue == "OTOMATIS") {
    modePhStr = strValue;
  }
}
void parsingModePpm(String strValue) {
  if (strValue == "MANUAL" || strValue == "OTOMATIS") {
    modePpmStr = strValue;
  }
}
void parsingIntervalOn(String strValue) {
  if (strValue.toInt() > 999 & strValue.toInt() < maksIntervalOn) {
    intervalOn = strValue.toInt();
    intervalPhOn = intervalOn;
    intervalPpmOn = intervalOn;
  }
}
void parsingIntervalOff(String strValue) {
  if (strValue.toInt() > 999 & strValue.toInt() < maksIntervalOff) {
    intervalOff = strValue.toInt();
    intervalPhOff = intervalOff;
    intervalPpmOff = intervalOff;
  }
}
void parsingIntervalLimit(String strValue) {
  if (strValue.toInt() > 6000 & strValue.toInt() < intervalLimit) {
    intervalLimit = strValue.toInt();
  }
}
void parsingBatasMarginPh(String strValue) {
  if (strValue.toFloat() > 0.2 & strValue.toFloat() < 4) {
    batasMarginPh = strValue.toFloat();
  }
}


void parsingInternalData() {

  schPpmStr = preferences.getString("schPpm");
  schPenyiramanStr = preferences.getString("schPen");
  String _modePhStr = preferences.getString("modePh");
  String _modePpmStr = preferences.getString("modePpm");
  String _targetPhStr = preferences.getString("targetPh");
  String _targetPpmStr = preferences.getString("targetPpm");
  String _intervalOnStr = preferences.getString("intervalOn");
  String _intervalOffStr = preferences.getString("intervalOff");
  String _intervalLimitStr = preferences.getString("intervalLimit");
  String _batasMarginPhStr = preferences.getString("batasMarginPh");

  parsingTargetPpm(_targetPpmStr);
  parsingTargetPh(_targetPhStr);
  parsingModePh(_modePhStr);
  parsingModePpm(_modePpmStr);
  parsingIntervalOn(_intervalOnStr);
  parsingIntervalOff(_intervalOffStr);
  parsingIntervalLimit(_intervalLimitStr);
  parsingBatasMarginPh(_batasMarginPhStr);

  JsonPreprocessorSchedulePpm(schPpmStr);
  JsonPreprocessorSchedulePenyiraman(schPenyiramanStr);
}

void printAllFromPrefs() {
  Serial.println("schPpm: " + schPpmStr);
  Serial.println("schPen: " + schPenyiramanStr);
  Serial.println("modePh: " + modePhStr);
  Serial.println("modePpm: " + modePpmStr);
  Serial.println("targetPh: " + String(targetPh));
  Serial.println("targetPpm: " + String(targetPpm));
  Serial.println("intervalLimitStr: " + String(intervalLimit));
  Serial.println("intervalOffStr: " + String(intervalOff));
  Serial.println("intervalOnStr: " + String(intervalOn));
  Serial.println("batasMarginPh: " + String(batasMarginPh));
  Serial.println("delay 5 sec");
}
