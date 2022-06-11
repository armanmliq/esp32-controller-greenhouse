bool beginPhMixing;
bool beginNutrisiMixing;
bool isFloatTrue, isFloatLimit, isPhMixingBegin, isPpmMixingBegin;
byte batasAtasPh;
byte batasBawahPh;
bool lastStatsLimitPenyiraman;
unsigned long lastMillisPenyiraman;
bool lastStatsLimitPengisian;
unsigned long lastMillisPengisian;
bool lastStatsLimitPpmUp;
unsigned long lastMillisPpmUp;
bool lastStatsLimitPhDown;
unsigned long lastMillisPhDown;
bool lastStatsLimitPhUp;
unsigned long lastMillisPhUp;
unsigned long phMillis, ppmMillis;
bool isToUp, isToDown;
bool phMixingExcBeginning;
bool isPhTooHigh, isPhTooLow;

void setupForPhMixing() {
  isPhTooHigh = false;
  isPhTooLow = false;
  phMixingExcBeginning = false;
  isPpmMixingBegin = false;
  isPhMixingBegin = true;
}
void setupForPpm() {
  isPpmMixingBegin = true;
  intervalPpm = intervalOnPpm;
  dispActivity("adjust nutrition abmix");
}
void detectForFilling() {
  if (floatStatus) {
    digitalWrite(RelayPompaPengisianPin, HIGH);
    isFloatTrue = true;
    isFloatLimit = false;
    isPhMixingBegin = false;
    isPhTooHigh = false;
    isPhTooLow = false;
    phMixingExcBeginning = false;
    isPpmMixingBegin = false;

    dispActivity("filling air baku");
  }
  if (isFloatTrue & !floatStatus & !isFloatLimit) {
    isFloatLimit = true;
    digitalWrite(RelayPompaPengisianPin, LOW);
    if (modePhStr == "OTOMATIS") {
      setupForPhMixing();
    }
  }
}
void detectForPhMixing() {
  if (isPhMixingBegin) {
    if (!phMixingExcBeginning) {
      phMixingExcBeginning = true;
      intervalPh = intervalOnPh;
      batasAtasPh = targetPh + batasMarginPh;
      batasBawahPh = targetPh - batasMarginPh;
      if (sensPh > batasAtasPh) {
        isPhTooHigh = true;
        isPhTooLow = false;
        dispActivity("ph mixing to " + String(targetPh));
      }
      if (sensPh < batasBawahPh) {
        isPhTooLow = true;
        isPhTooHigh = false;
        dispActivity("ph mixing to " + String(targetPh));
      }
    }
    if (millis() - phMillis > intervalPh) {
      phMillis = millis();
      bool isPompaPhUpOn = digitalRead(RelayPompaPhUpPin);
      bool isPompaPhDownOn = digitalRead(RelayPompaPhDownPin);
      //isPhLow?
      if (isPhTooLow) {
        if (sensPh >= targetPh) {
          dispActivity("ph already to target");
          isPhMixingBegin = false;
          digitalWrite(RelayPompaPhUpPin, LOW);
          if (modePpmStr == "OTOMATIS") {
            setupForPpm();
          }
          return;
        }
        if (isPompaPhUpOn) {
          intervalPh = intervalOffPh;
          digitalWrite(RelayPompaPhUpPin, LOW);
        } else {
          intervalPh = intervalOnPh;
          digitalWrite(RelayPompaPhUpPin, HIGH);
        }
        digitalWrite(RelayPompaPhDownPin, LOW);
      }
      if (isPhTooHigh) {
        if (sensPh <= targetPh) {
          dispActivity("ph adjustment done");
          isPhMixingBegin = false;
          digitalWrite(RelayPompaPhDownPin, LOW);
          if (modePpmStr == "OTOMATIS") {
            setupForPpm();
          }
          return;
        }
        if (isPompaPhDownOn) {
          intervalPh = intervalOffPh;
          digitalWrite(RelayPompaPhDownPin, LOW);
        } else {
          intervalPh = intervalOnPh;
          digitalWrite(RelayPompaPhDownPin, HIGH);
        }
        digitalWrite(RelayPompaPhUpPin, LOW);
      }
    }
  }
}
void detectForPpmMixing() {
  if (isPpmMixingBegin) {
    if (sensPpm > targetPpm) {
      digitalWrite(RelayPompaPpmUpPin, LOW);
      isPpmMixingBegin = false;
      dispActivity("nutrition already to target");
      return;
    }
    if (millis() - ppmMillis > intervalPpm) {
      ppmMillis = millis();
      bool isPompaPpmOn = digitalRead(RelayPompaPpmUpPin);
      if (isPompaPpmOn) {
        intervalPpm = intervalOffPpm;
        digitalWrite(RelayPompaPpmUpPin, LOW);
      } else {
        intervalPpm = intervalOnPpm;
        digitalWrite(RelayPompaPpmUpPin, HIGH);
      }
    }
  }
}
void readPh() {
  sensPh = random(4, 6);
}
void readPpm() {
  sensPpm = random(800, 1200);
}
void readHumidity() {
  sensHumidity = random(60, 90);
}
void readTempWater() {
  sensWaterTemp = random(30, 40);
}
void readTempRoom() {
  sensTempRoom = random(25, 40);
}
void readFloat() {
  floatStatus = digitalRead(floatSensorPin);
  detectForFilling();
}

void readSensor() {
  readFloat();
  readPh();
  readPpm();
  readHumidity();
  readTempWater();
  readTempRoom();
}
void validationTargetPhPpm() {
  if (targetPh > 12 || targetPh < 1) {
    targetPh = 5.5;
  }
  if (targetPpm > 2000 || targetPpm < 1) {
    targetPpm = 1000;
  }
}
void limitActivePenyiraman() {
  if (pompaPenyiramanStats) {
    //    Serial.println("pompaPenyiramanStatsHIGH");
    delay(100);
    if (pompaPenyiramanStats != lastStatsLimitPenyiraman) {
      lastStatsLimitPenyiraman = pompaPenyiramanStats;
      lastMillisPenyiraman = millis();
    }
    if (millis() - lastMillisPenyiraman > intervalLimit) {
      digitalWrite(RelayPompaPenyiramanPin, LOW);
      pompaPenyiramanStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPenyiraman = millis();
  }
}
void limitActivePhUp() {
  if (pompaPhUpStats) {
    //    Serial.println("pompaPhUpStatsHIGH");
    delay(100);
    if (pompaPhUpStats != lastStatsLimitPhUp) {
      lastStatsLimitPhUp = pompaPhUpStats;
      lastMillisPhUp = millis();
    }
    if (millis() - lastMillisPhUp > intervalLimit) {
      digitalWrite(RelayPompaPhUpPin, LOW);
      pompaPhUpStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPhUp = millis();
  }
}
void limitActivePhDown() {
  if (pompaPhDownStats) {
    //    Serial.println("pompaPhDownStatsHIGH");
    delay(100);
    if (pompaPhDownStats != lastStatsLimitPhDown) {
      lastStatsLimitPhDown = pompaPhDownStats;
      lastMillisPhDown = millis();
    }
    if (millis() - lastMillisPhDown > intervalLimit) {
      digitalWrite(RelayPompaPhDownPin, LOW);
      pompaPhDownStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPhDown = millis();
  }
}


void limitActivePpmUp() {
  if (pompaPpmUpStats) {
    //    Serial.println("pompaPpmUpStatsHIGH");
    delay(100);
    if (pompaPpmUpStats != lastStatsLimitPpmUp) {
      lastStatsLimitPpmUp = pompaPpmUpStats;
      lastMillisPpmUp = millis();
    }
    if (millis() - lastMillisPpmUp > intervalLimit) {
      digitalWrite(RelayPompaPpmUpPin, LOW);
      pompaPpmUpStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPpmUp = millis();
  }
}
void limitActivePengisian() {
  if (pompaPengisianStats) {
    if (pompaPengisianStats != lastStatsLimitPengisian) {
      lastStatsLimitPengisian = pompaPengisianStats;
      lastMillisPengisian = millis();
    }
    if (millis() - lastMillisPengisian > intervalLimit) {
      digitalWrite(RelayPompaPengisianPin, LOW);
      pompaPengisianStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPengisian = millis();
  }
}
void limitAll() {
  limitActivePengisian();
  limitActivePpmUp();
  limitActivePhDown();
  limitActivePhUp();
  limitActivePenyiraman();
}
