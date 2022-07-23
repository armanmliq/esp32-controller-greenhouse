#define API_KEY "AIzaSyBwu8sXyJOM-uu-NB9kfjNkIrnDkMKPoco"
#define DATABASE_URL "https://esp32-7fe29-default-rtdb.firebaseio.com"
#define USER_EMAIL "armanmaliq92@gmail.com"
#define USER_PASSWORD "armanmaliq92@gmail.com"

unsigned long sendDataPrevMillis = 0;
String MainPathsetParameter = "/users/" + uid + "/set_parameter/";
String childPathSetParameter[25] = {"set_interval_notif_oto", "set_interval_limit", "set_interval_detect_all", "set_interval_report", "set_interval_cek_ppm", "set_interval_cek_ph", "set_margin_ppm", "set_batas_margin_ph", "set_interval_off_ph", "set_interval_off_ppm", "set_interval_on_ppm", "set_interval_on_ph", "set_pompa_ppm_up", "set_pompa_penyiraman", "set_mode_ppm", "set_ph", "set_ppm", "set_mode_ph", "scheduler_ppm_str", "scheduler_jadwal_penyiraman", "set_pompa_ph_down", "set_pompa_ph_up", "set_pompa_pengisian", "set_sprayer"};

FirebaseData fbdo;
FirebaseData streamParameter;
FirebaseAuth auth;
FirebaseConfig config;

void streamCallbackParameter(MultiPathStreamData stream)
{
  size_t numChild = sizeof(childPathSetParameter) / sizeof(childPathSetParameter[0]);
  for (size_t i = 0; i < numChild; i++)
  {
    if (stream.get(childPathSetParameter[i]))
    {
      String path = stream.dataPath;
      String _data = stream.value;

      Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : "");
      //cek_scheduler_ppm
      if (path.indexOf("scheduler_ppm_str") > -1) {
        preferences.putString("schPpm", _data);
        JsonPreprocessorSchedulePpm(_data);

        //scheduler_jadwal_penyiraman
      } else if (path.indexOf("scheduler_jadwal_penyiraman") > -1) {
        preferences.putString("schPen", _data);
        JsonPreprocessorSchedulePenyiraman(_data);
      } else {
        //direct proccess no json
        toOutputResponse(path, _data);
      }

    }
  }
}
void streamTimeoutCallbackParameter(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");
  if (!streamParameter.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", streamParameter.httpCode(), streamParameter.errorReason().c_str());
}
void setup_firebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void begin_stream() {
  Serial.println(" <=========> begin_stream <===========> ");
  if (!Firebase.beginMultiPathStream(streamParameter, MainPathsetParameter))
    Serial.printf("sream begin error, %s\n\n", streamParameter.errorReason().c_str());
  Firebase.setMultiPathStreamCallback(streamParameter, streamCallbackParameter, streamTimeoutCallbackParameter);
  config.timeout.wifiReconnect = 10 * 1000;
  config.timeout.rtdbStreamReconnect = 10 * 1000;
  config.timeout.rtdbStreamError = 10 * 1000;
}
