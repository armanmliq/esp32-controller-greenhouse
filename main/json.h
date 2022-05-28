#include <ArduinoJson.h>

struct Departure {
  char destination[32];
  char departure[32];
  int  deviation;
} depart;

struct Departure dep_arr[3];
String ListDateFrom[] = {};
void JsonPreprocessor(String json)
{
  StaticJsonDocument<1024> doc;
  //String input = "{\"ResponseData\":{\"Metros\":[{\"Destination\":\"Destination_1\",\"ExpectedDateTime\":\"2021-04-29T05:15:30\"},{\"Destination\":\"Destination_2\",\"ExpectedDateTime\":\"2021-04-29T05:21:30\"},{\"Destination\":\"Destination_2\",\"ExpectedDateTime\":\"2021-04-29T05:30:30\"},{\"Destination\":\"Destination_1\",\"ExpectedDateTime\":\"2021-04-29T05:36:30\"}]}}";
  //String input = "{\"data\":[{\"id\":\"2022-05-28 17:14:52.266983\",\"dateFrom\":\"2022-06-10\",\"dateTo\":\"2022-06-17\",\"ppm\":\"55\"}]}";
  //Serial.println(String(json.length()) + " " + json);
  //Serial.println(String(input.length()) + " " + input);
  //remove slash
  json.replace("\\","");
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  byte index = 0;
  for (JsonObject Data : doc["data"].as<JsonArray>()) {
    index++;
    String dateFrom = Data["dateFrom"];
    String dateTo = Data["dateTo"];
    ListDateFrom[index] = dateFrom;
    Serial.println(dateFrom);
  }


}
