byte tick;
byte updateRate = 4;
String dispTimeStr;

void displayTime() {
  dispTimeStr = "";
  if (globalHour < 10) {
    dispTimeStr += '0';
  }
  dispTimeStr += String(globalHour);
  dispTimeStr += ':';
  if (globalMinute < 10) {
    dispTimeStr += '0';
  }
  dispTimeStr += String(globalMinute);
  lcd.print(dispTimeStr);
}
void displaySensor() {

  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print("ph  :" + String(sensPh,1) + " pH");
  lcd.setCursor(15, 0);

  displayTime();

  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("tds :" + String(sensPpm,0) + " ppm");
}

void setDispIndex(byte _dispIndex) {
  tick = 0;
  dispIndex = _dispIndex;
}
void displayInfo() {
  tick++;

  if (tick >= updateRate) {
    tick = 0;
    dispIndex++;
    if (dispIndex >= 4) dispIndex = 0;
  }

  switch (dispIndex) {
    case 0:
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print("mode ph :" + modePhStr);
      break;
    case 1:
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print("mode ppm :" + modePpmStr);
      break;
    case 2:
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print("target ppm:" + targetPpmStr);
      break;
    case 3:
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print("target ph:" + targetPhStr);
      break;
  }
}
void dispActivity(String _text) {

  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print(_text);
}
