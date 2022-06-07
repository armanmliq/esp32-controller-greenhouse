bool beginPhMixing;
bool beginNutrisiMixing;
bool isFloatTrue, isFloatLimit, isPhMixingBegin, isPpmMixingBegin;
byte batasAtasPh;
byte batasBawahPh;
float marginBatasPh = 0.5;

unsigned int intervalPhOn = 5000;
unsigned int intervalPhOff = 3000;
unsigned int intervalPh;
unsigned int intervalPpmOn = 5000;
unsigned int intervalPpmOff = 3000;
unsigned int intervalPpm;

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
  intervalPpm = intervalPpmOn;
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
    setupForPhMixing();

  }
}
void detectForPhMixing() {
  if (isPhMixingBegin) {
    if (!phMixingExcBeginning) {
      phMixingExcBeginning = true;
      intervalPh = intervalPhOn;
      batasAtasPh = targetPh + marginBatasPh;
      batasBawahPh = targetPh - marginBatasPh;
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
          setupForPpm();
          return;
        }
        if (isPompaPhUpOn) {
          intervalPh = intervalPhOff;
          digitalWrite(RelayPompaPhUpPin, LOW);
        } else {
          intervalPh = intervalPhOn;
          digitalWrite(RelayPompaPhUpPin, HIGH);
        }
        digitalWrite(RelayPompaPhDownPin, LOW);
      }
      if (isPhTooHigh) {
        if (sensPh <= targetPh) {
          dispActivity("ph adjustment done");
          isPhMixingBegin = false;
          digitalWrite(RelayPompaPhDownPin, LOW);
          setupForPpm();
          return;
        }
        if (isPompaPhDownOn) {
          intervalPh = intervalPhOff;
          digitalWrite(RelayPompaPhDownPin, LOW);
        } else {
          intervalPh = intervalPhOn;
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
        intervalPpm = intervalPpmOff;
        digitalWrite(RelayPompaPpmUpPin, LOW);
      } else {
        intervalPpm = intervalPpmOn;
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
  sensHumidity = random(25, 40);
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
