#include <ArduinoJson.h>
#include "getvalue.h"
#include "schedule_ppm.h"
#include "schedule_penyiraman.h"
StaticJsonDocument<2048> docPpm;
StaticJsonDocument<2048> docPenyiraman;

void JsonPreprocessorSchedulePpm(String json)
{

  /*
    String json :

    {
    "data": [
      {
        "id": "2022-05-29 01:09:35.182323",
        "dateFrom": "2022-06-03",
        "dateTo": "2022-06-04",
        "ppm": "66"
      },
      {
        "id": "2022-05-29 01:16:06.998454",
        "dateFrom": "2022-06-08",
        "dateTo": "2022-06-09",
        "ppm": "58"
      }
    ]
    }
  */
  //remove unwanted slash incoming data, if not removed deserialization error
  json.replace("\\", "");

  //proccess json and check error
  DeserializationError error = deserializeJson(docPpm, json);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  //data isNotEmpty? start clear old data
  if (json.indexOf("dateFrom") != -1) {
    indexSchedulePpm = 0;
    listEpochStartPpm = "";
    listEpochEndPpm = "";
    listPpm = "";
  } else {
    return;
  }

  //LOOPING ITERABLE JSON
  for (JsonObject Data : docPpm["data"].as<JsonArray>()) {
    indexSchedulePpm ++;
    String dateStart = Data["dateFrom"];
    String dateEnd = Data["dateTo"];
    String ppm = Data["ppm"];

    //parsing and add each element
    parseDateStart(dateStart);
    parseDateEnd(dateEnd);
    parsePpm(ppm);
  }

  Serial.println("result ppm");
  //Proccess to check data and control output
  CheckSchedulePpm();

  //send info
  if (millis() > 30000) {
    setAktifitas("menyimpan jadwal nutrisi -> " + String(indexSchedulePpm) + " item");
  }

}

void JsonPreprocessorSchedulePenyiraman(String json)
{
  //remove unwanted slash incoming data, if not removed deserialization error
  json.replace("\\", "");

  
  //deserializeJson and check error
  DeserializationError error = deserializeJson(docPenyiraman, json);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  //data isNotEmpty? start clear old data
  if (json.indexOf("id") != -1) {
    indexSchedulePenyiraman = 0;
    listEpochEndDatePenyiraman = "";
    listEpochStartDatePenyiraman = "";
  } else {
    //data empty mean penyiraman off
    dispPenyiramanStr = "";
    indexSchedulePenyiraman = 0;
    digitalWrite(RelayPompaPenyiramanPin, !LOW);
    return;
  }
  
  //LOOPING ITERABLE JSON
  for (JsonObject Data : docPenyiraman["data"].as<JsonArray>())
  {
    indexSchedulePenyiraman ++;
    String TimeOfDayStr = Data["TimeOfDay"];
    String LamaPenyiramanStr = Data["LamaPenyiraman"];
    parseSchedulePenyiraman(TimeOfDayStr, LamaPenyiramanStr);
  }
  
  //  //Proccess to check data and control output
  CheckSchedulePenyiraman();
 
  //send info 
  if (millis() > 30000) {
    setAktifitas("menyimpan jadwal penyiraman -> " + String(indexSchedulePenyiraman) + " item");
  }
}
