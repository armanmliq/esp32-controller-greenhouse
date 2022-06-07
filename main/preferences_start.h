
void parsingInternalData(){ 
  schPpmStr = preferences.getString("schPpm");
  schPenyiramanStr = preferences.getString("schPen");
  modePhStr = preferences.getString("modePh");
  modePpmStr = preferences.getString("modePpm");
  targetPhStr = preferences.getString("targetPh");
  targetPpmStr = preferences.getString("targetPpm"); 

  targetPh = targetPhStr.toFloat();
  targetPpm = targetPpmStr.toFloat();
  validationTargetPhPpm();
  
  Serial.println("schPpm: " + schPpmStr); 
  Serial.println("schPen: " + schPenyiramanStr);
  Serial.println("modePh: " + modePhStr);
  Serial.println("modePpm: " + modePpmStr);
  Serial.println("targetPh: " + targetPhStr);
  Serial.println("targetPpm: " + targetPpmStr);
  Serial.println("delay 5 sec");
  JsonPreprocessorSchedulePpm(schPpmStr);
  JsonPreprocessorSchedulePenyiraman(schPenyiramanStr);
}
