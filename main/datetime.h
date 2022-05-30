#include "TimeLib.h"
long epochNow = 0;

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


void updateNtp(){
  Serial.println();
 int offsetGmt = 3600 * 7;
 epochNow = timeClient.getEpochTime() + offsetGmt;
 Serial.println(epochNow);
}


void digitalClockDisplay(){
  Serial.print(hour(epochNow));
  Serial.print(':');
  Serial.println(minute(epochNow));
}
