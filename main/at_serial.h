void serial(){
  if(Serial.available() > 0)
  {
    char dat = Serial.read();
    if(dat == 'a')
    {
      updateAllGrafik();
      Serial.println("response");
    }
  
   if(dat == 'd')
    {
      Serial.println("delete proccess");
      coreDeleteGrafik();
    }
    
   if(dat == 'p'){
    Serial.println("config time");
    configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
   }
 }
}
