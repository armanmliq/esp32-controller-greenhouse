#include <Preferences.h>
Preferences preferences;

//void saveFloat(String _key, float _data){
//  preferences.putFloat(_key, _data);
//}
//
//String saveString(String _key, String _data){
//  preferences.putFloat(_key, _data);  
//}

void readPreferences(){
  String schPpm = preferences.getString("schPpm");
  String schPen = preferences.getString("schPen");
  String modePh = preferences.getString("modePh");
  String modePpm = preferences.getString("modePpm");
  String targetPh = preferences.getString("targetPh");
  String targetPpm = preferences.getString("targetPpm");
  
  Serial.println("schPpm: " + schPpm);
  Serial.println("schPen: " + schPen);
  Serial.println("modePh: " + modePh);
  Serial.println("modePpm: " + modePpm);
  Serial.println("targetPh: " + targetPh);
  Serial.println("targetPpm: " + targetPpm);
}
