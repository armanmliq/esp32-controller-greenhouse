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
  //  dispTimeStr += ':';
  //  if (globalSec < 10) {
  //    dispTimeStr += '0';
  //  }
  //  dispTimeStr += String(globalSec);

  lcd.setCursor(0, 0);
  lcd.print(dispTimeStr);
}

void displaySensor() {
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(9 , 0);
  lcd.print("T.uda:" + String(sensTempRoom, 1) + "C");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("ppm:" + String(sensPpm, 0));
  lcd.setCursor(9, 1);
  lcd.print("T.air:" + String(sensTempWater, 1) + "C");
  lcd.setCursor(0, 2);
  lcd.print("pH :" + String(sensPh, 1) + " ");
  lcd.setCursor(9, 2);
  lcd.print("Kelem:" + String(sensHumidity, 1) + "%");
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
    if (dispIndex >= 6) dispIndex = 0;
  }

  switch (dispIndex) {
    case 0:
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print("MODE-PH:" + modePhStr);
      break;
    case 1:
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print("MODE-PPM:" + modePpmStr);
      break;
    case 2:
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print("SET-PPM:" + String(targetPpm, 0) + " ");
      break;
    case 3:
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print("SET-PH:" + String(targetPh, 1) + " ");
      break;
    case 4:
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print(aktivitasPh);
      break;
    case 5:
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print(aktivitasPpm);
      break;
  }
}

void dispActivity(String _text) {
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print(_text);
}
