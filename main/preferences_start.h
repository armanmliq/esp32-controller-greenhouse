void parsingInternalData(){ 
  schPpmStr = preferences.getString("schPpm");
  schPenyiramanStr = preferences.getString("schPen");
  modePhStr = preferences.getString("modePh");
  modePpmStr = preferences.getString("modePpm");
  targetPhStr = preferences.getString("targetPh");
  targetPpmStr = preferences.getString("targetPpm"); 

  targetPh = targetPhStr.toFloat();
  targetPpm = targetPpmStr.toFloat();
  
  JsonPreprocessorSchedulePpm(schPpmStr);
  JsonPreprocessorSchedulePenyiraman(schPenyiramanStr);
}
