
#include <HTTPClient.h>
void sendFcmNotif() {
  if (isSendNotif) {
    isSendNotif = false;
    HTTPClient http;

    if (internetStats) { 
      //Check WiFi connection status
      vTaskSuspend(TaskHandle_3);
      http.begin("https://fcm.googleapis.com/fcm/send");  //Specify destination for HTTP request
      http.addHeader("Content-Type", "application/json");             //Specify content-type header
      http.addHeader("Authorization", "key=AAAA1qy7mrg:APA91bHF83QPgcK2Q5BhG30J851ejW_GFy0N-_OUKNqgBMP_8EmaBwicjMK2W2vAWK-c8pzGEa2vuzJMTeQwOE5YD5q-4bfK9XYxlqu7QflA2ynzuf3gDPOVmU9VJC8BiSglgOAm2ehP");             //Specify content-type header
      String json = "{\"to\": \"/topics/GB12AsiGsmPhPRVeOlFD9Uv6oHm1\",\"notification\":{\"body\" : \"" + body + "\",\"title\": \"" + title + "\"}}";
      int httpResponseCode = http.POST(json);   //Send the actual POST request
      if (httpResponseCode > 0) {
        String response = http.getString();                       //Get the response to the request
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer 
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode); 
      }
      http.end();  //Free resources
      vTaskSuspend(TaskHandle_4);
      vTaskResume(TaskHandle_3);
    } else {
      Serial.println("no fcm on WiFi failed");
    }
  }
}
