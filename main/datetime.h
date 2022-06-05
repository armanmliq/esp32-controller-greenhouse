#include "TimeLib.h"
long epochNow = 0;

#include <NTPClient.h>
#include <WiFiUdp.h> 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP); 
 
void digitalClockDisplay(){
  Serial.print(day(timeClient.getEpochTime() + offsetGmt));
  Serial.print('-');
  Serial.print(month(timeClient.getEpochTime() + offsetGmt));
  Serial.print('-');
  Serial.print(year(timeClient.getEpochTime() + offsetGmt));
  Serial.print(' '); 
  Serial.print(hour(timeClient.getEpochTime() + offsetGmt));
  Serial.print(':');
  Serial.println(String(minute(timeClient.getEpochTime()) + offsetGmt) + " "+String(WiFi.status()));
}

void updateNtp(){
  //start Time
 timeClient.begin();
 timeClient.update();  
 epochNow = timeClient.getEpochTime() + offsetGmt;
 digitalClockDisplay();
}
