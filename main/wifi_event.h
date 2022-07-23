
bool isSetupFirebaseEnable;
void wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Reconnecting...");
  WiFi.disconnect();
  delay(200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void wifi_connected(WiFiEvent_t event, WiFiEventInfo_t info) {
  //  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  //  timeClient.setTimeOffset(7 * 3600);
  updateNtp();
  //  setup_firebase();
  Serial.println("<=================>>>> CONNECTED <<<<=================> ");
}
