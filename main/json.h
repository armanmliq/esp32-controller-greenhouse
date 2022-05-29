#include <ArduinoJson.h>
#include "parsing_jadwal_ppm.h"
StaticJsonDocument<1024> doc;

void JsonPreprocessor(String json)
{
  //remove unwanted slash
  json.replace("\\","");

  //proccess json and check error
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  //data isNotEmpty? start clear old data
    if(json.indexOf("dateFrom") != -1){
      _indexSchedulePpm = 0;
      listStartDate = "";
      listEndDate = "";
      listPpm ="";      
    } 

  //LOOPING ITERABLE JSON
  for (JsonObject Data : doc["data"].as<JsonArray>()) {
    _indexSchedulePpm ++;
    String dateStart = Data["dateFrom"];
    String dateEnd = Data["dateTo"]; 
    String ppm = Data["ppm"];
    parseDateStart(dateStart);
    parseDateEnd(dateEnd);
    parsePpm(ppm);
  }

  //Proccess to check data and control output
  //Serial.println("total index = "+ String(_index));
  //Serial.println("parseDateStart = "+ listStartDate);
  //Serial.println("parseDateEnd = "+ listEndDate);
  //Serial.println("ppm = "+ listPpm);
  CheckSchedulePpm();
}
