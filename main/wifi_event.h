bool isSetupFirebaseEnable;
void wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Reconnecting..."); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  isSetupFirebaseEnable = 1;
  updateNtp();   
}

void wifi_connected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("connected"); 
}
void setupFirebaseAtWifiDisconnect(){
  if(WiFi.status() == WL_CONNECTED & isSetupFirebaseEnable){
    Serial.println("resetup firebase");
    isSetupFirebaseEnable = 0;
    updateNtp();   
  }
}
