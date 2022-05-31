void serial(){
  if(Serial.available() > 0)
  {
    char dat = Serial.read();
    if(dat == 'x')
    {
      coreUpdateAllGrafik();
      Serial.println("response");
    }
  
   if(dat == 'a')
    {
      coreDeleteUpGrafik();
    }
    
    if(dat == 'b')
    {

    } 
    
    if(dat == 'c')
    {

    }
  


 }
}
