void displaySensor(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PH:" +String(6.5));
  lcd.setCursor(0,1);
  lcd.print("PPM:" +String(1020));
  lcd.setCursor(0,2);
  lcd.print("MODE PH:"+modePhStr);
  lcd.setCursor(0,3);
  lcd.print("MODE PPM:" +modePpmStr);

}

void displayTime(){
   DateTime now = rtc.now();
}
