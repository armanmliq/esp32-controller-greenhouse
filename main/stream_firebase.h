#include "json.h"
void streamCallbackParameter(MultiPathStreamData stream)
{
  size_t numChild = sizeof(childPathSetParameter) / sizeof(childPathSetParameter[0]);
  for (size_t i = 0; i < numChild; i++)
  {
    if (stream.get(childPathSetParameter[i]))
    { 
      String path = stream.dataPath;
   Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : "");
      //cek_scheduler_ppm
      if(path.indexOf("scheduler_ppm_str") > -1){
            JsonPreprocessorSchedulePpm(stream.value);        
        }else if(path.indexOf("scheduler_jadwal_penyiraman") > -1){
            //Serial.println("><><>scheduler_jadwal_penyiraman<><><>");
            JsonPreprocessorSchedulePenyiraman(stream.value);        
        }else{
          Serial.println("wrong path!! " +String( path.indexOf("scheduler_jadwal_penyiraman")));
        }
    }
  }
//  Serial.println();
//  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
}

void streamTimeoutCallbackParameter(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!streamParameter.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", streamParameter.httpCode(), streamParameter.errorReason().c_str());
}
