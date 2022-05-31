void serial(){
  if(Serial.available() > 0)
  {
    char dat = Serial.read();
    if(dat == 'a')
    {
      updateAllGrafik();
//      coreUpdateAllGrafik();
      Serial.println("response");
    }
  
   if(dat == 'd')
    {
//      deleteGrafikDataLastOneDay();
      coreDeleteGrafik();
    }
    
   


 }
}
