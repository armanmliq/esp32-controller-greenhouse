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
     String sv = "44";
     preferences.putString("counter: ", sv);
   }
 }
}
