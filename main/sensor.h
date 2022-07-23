bool beginPhMixing;
bool beginNutrisiMixing;
bool isFloatTrue, isFloatLimit, isPhMixingBegin, isPpmMixingBegin;
float batasAtasPh;
float batasBawahPh;
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
bool isPpmTooHigh, isPpmTooLow;
bool savedStateAktifitas;
bool limSendCalculatePpmInfo;
bool limSendNotifModePh;
bool limSendCalculatePhInfo;
int batasAtasPpm;
int batasBawahPpm;
bool limSendNotifTandonHabis;


float roundToOneDecPoint(float floatVal) {
  float floatOnePoint;
  long val;
  val = (long) (floatVal * 10L);
  floatOnePoint = (float) val / 10.0;
  return floatOnePoint;
}

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, HIGH); //Switch to transmit data
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, LOW); //Switch to receive data
}

void setupSht20() {
  pinMode(MAX485_RE_NEG, OUTPUT);
  digitalWrite(MAX485_RE_NEG, LOW);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  modbus.begin(Slave_ID, Serial2);
  modbus.preTransmission(preTransmission);
  modbus.postTransmission(postTransmission);

}

void detectForFilling() {
  if (sensFloat & !limSendNotifTandonHabis) {
    limSendNotifTandonHabis = true;
    Serial.println("air tandon habis, pompa tandon dihidupkan");
    setAktifitas("warning! air tandon habis,\npompa tandon dihidupkan");
    digitalWrite(RelayPompaPengisianPin, !HIGH);
    isFloatTrue = true;
    isFloatLimit = false;
    isPhMixingBegin = false;
    isPhTooHigh = false;
    isPhTooLow = false;
    //    dispActivity("pompa tandon on");
  }
  if (isFloatTrue & !sensFloat & !isFloatLimit) {
    limSendNotifTandonHabis = false;
    Serial.println("pengisian tandon selesai");
    setAktifitas("pengisian selesai,\nmematikan pompa tandon"); 
    isFloatLimit = true;
    digitalWrite(RelayPompaPengisianPin, !LOW);
  }
}

void ppmTooLowOrHigh() {
  if (sensPpm > batasAtasPpm) {
    isPpmTooHigh = true;
    isPpmTooLow = false;
    Serial.println("menurunkan ppm " + String(sensPpm, 0) + " --> " + String(targetPpm, 0));
    setAktifitas("menurunkan level ppm " + String(sensPpm, 0) + " --> " + String(targetPpm, 0));
    aktivitasPpm = "menurunkan ppm";
  } else if (sensPpm < batasBawahPpm) {
    isPpmTooLow = true;
    isPpmTooHigh = false;
    aktivitasPpm = "menaikkan ppm";
    setAktifitas("menaikkan level ppm " + String(sensPpm, 0) + " --> " + String(targetPpm, 0));
    Serial.println("menaikkan ppm " + String(sensPpm, 0) + " --> " + String(targetPpm, 0));
  } else {
    isPpmTooHigh = false;
    isPpmTooLow = false;
    aktivitasPpm = "ppm mencapai target";
    setAktifitas("level ppm di area toleransi, skip control..");
    return;
  }
}
void phTooLowOrHigh() {
  if (sensPh > batasAtasPh) {
    isPhTooHigh = true;
    isPhTooLow = false;
    Serial.println("menurunkan ph " + String(sensPh) + " --> " + String(targetPh));
    setAktifitas("menurunkan level ph " + String(sensPh) + " --> " + String(targetPh));
    aktivitasPh = "menurunkan ph";
  } else if (sensPh < batasBawahPh) {
    isPhTooLow = true;
    isPhTooHigh = false;
    aktivitasPh = "menaikkan ph";
    setAktifitas("menaikkan level ph " + String(sensPh) + " --> " + String(targetPh));
    Serial.println("menaikkan ph " + String(sensPh) + " --> " + String(targetPh));
  } else {
    isPhTooHigh = false;
    isPhTooLow = false;
    aktivitasPpm = "ph mencapai target";
    setAktifitas("level ph di area toleransi, skip control..");
    return;
  }
}
void detectForPhMixing() {
  int batasAmanAtas = 12;
  int batasAmanBawah = 2;
  bool isPhNormal = sensPh >= batasAmanBawah & sensPh <= batasAmanAtas ? true : false;
  if (isPhMixingBegin) {

    if (!limSendNotifModePh & isPhNormal) {
      limSendNotifModePh = true;
      if (modePhStr != "OTOMATIS") {
        Serial.println("skip atur ph pada mode manual..");
        setAktifitas("skip atur ph pada mode manual..");
        return;
      } else {
        limSendCalculatePhInfo = false;
        isPhTooHigh = false;
        isPhTooLow = false;
        intervalPh = intervalOnPh;
        batasAtasPh = targetPh + batasMarginPh;
        batasBawahPh = targetPh - batasMarginPh;
        Serial.println("mode otomatis ph aktif, atas:" + String(batasAtasPh) + " bawah:" + String(batasBawahPh) + " interval: " + String(intervalOnPh));
        phTooLowOrHigh();
      }
    }

    if (millis() - mCalcPh > intervalNotifOtomatisPh) {
      if (modePhStr == "OTOMATIS") {
        mCalcPh = millis();
        if (!isPhNormal) {
          aktivitasPh = "atur ph dimatikan";
          setAktifitas("pengatur ph dimatikan sementara, range aman ph: " + String(batasAmanBawah) + " - " + String(batasAmanAtas) + ", cek sensor atau silahkan atur ph air secara manual terlebih dahulu..");
          digitalWrite(RelayPompaPhDownPin, !LOW);
          digitalWrite(RelayPompaPhUpPin, !LOW);
          isPhMixingBegin = false;
          return;
        } else {
          //          checking every intervalNotifOtomatisPh
          phTooLowOrHigh();
        }
      }
    }

    if (millis() - phMillis > intervalPh & modePhStr == "OTOMATIS" & isPhNormal) {
      phMillis = millis();
      bool isPompaPhUpOn = !digitalRead(RelayPompaPhUpPin);
      bool isPompaPhDownOn = !digitalRead(RelayPompaPhDownPin);
      //isPhLow?
      if (isPhTooLow) {
        Serial.println("PH TOO LOW");
        digitalWrite(RelayPompaPhDownPin, !LOW);
        if (sensPh >= targetPh) {
          limSendCalculatePhInfo = false;
          aktivitasPh = "atur ph selesai";
          setAktifitas("ATUR PH KE " + String(targetPh) + " SELESAI");
          Serial.println("ATUR PH KE " + String(targetPh) + " SELESAI");
          isPhMixingBegin = false;
          digitalWrite(RelayPompaPhUpPin, !LOW);
          return;
        }
        if (isPompaPhUpOn) {
          intervalPh = intervalOffPh;
          digitalWrite(RelayPompaPhUpPin, !LOW);
        } else {
          intervalPh = intervalOnPh;
          digitalWrite(RelayPompaPhUpPin, !HIGH);
        }
      }
      if (isPhTooHigh) {
        digitalWrite(RelayPompaPhUpPin, !LOW);
        Serial.println("PH TOO HIGH");
        if (sensPh <= targetPh) {
          limSendCalculatePhInfo = false;
          aktivitasPh = "atur ph selesai";
          Serial.println("ATUR PH KE " + String(targetPh) + " SELESAI");
          setAktifitas("ATUR PH KE " + String(targetPh) + " SELESAI");
          isPhMixingBegin = false;
          digitalWrite(RelayPompaPhDownPin, !LOW);
          return;
        }
        if (isPompaPhDownOn) {
          intervalPh = intervalOffPh;
          digitalWrite(RelayPompaPhDownPin, !LOW);
        } else {
          intervalPh = intervalOnPh;
          digitalWrite(RelayPompaPhDownPin, !HIGH);
        }
      }
    }
  }
}

bool limSendNotifModePpm;
void detectForPpmMixing() {
  int batasAmanAtas = 1300;
  int batasAmanBawah = 5;
  bool isPpmNormal = sensPpm >= batasAmanBawah & sensPpm <= batasAmanAtas ? true : false;
  if (isPpmMixingBegin) {
    if (millis() - mCalcPpm > intervalNotifOtomatisPpm) {
      if (modePpmStr == "OTOMATIS") {
        mCalcPpm = millis();
        if (!isPpmNormal) {
          aktivitasPpm = "atur ppm dimatikan";
          setAktifitas("pengatur ppm dimatikan sementara,range aman ppm: " + String(batasAmanBawah) + " - " + String(batasAmanAtas) + ", cek sensor atau silahkan atur ppm air secara manual terlebih dahulu..");
          digitalWrite(RelayPompaPpmDownPin, !LOW);
          digitalWrite(RelayPompaPpmUpPin, !LOW);
          isPpmMixingBegin = false;
          return;
        } else {
          ppmTooLowOrHigh();
        }
      }
    }
    if (!limSendNotifModePpm & isPpmNormal) {
      limSendNotifModePpm = true;
      if (modePpmStr != "OTOMATIS") {
        Serial.println("skip atur ppm pada mode manual..");
        setAktifitas("skip atur ppm pada mode manual..");
        return;
      } else {
        limSendCalculatePpmInfo = false;
        isPpmTooHigh = false;
        isPpmTooLow = false;
        intervalPpm = intervalOnPpm;
        batasAtasPpm = targetPpm + batasMarginPpm;
        batasBawahPpm = targetPpm - batasMarginPpm;
        Serial.println("pengaturan ppm pada mode otomatis dimulai.. atas:" + String(batasAtasPpm) + " bawah:" + String(batasBawahPpm) + " interval: " + String(intervalOnPpm));
      }
    }
    if (!limSendCalculatePpmInfo & modePpmStr == "OTOMATIS" & isPpmNormal) {
      limSendCalculatePpmInfo = true;
      Serial.println("CEK PPM -> TARGET:" + String(targetPpm, 0) + " BATAS_ATAS:" + String(batasAtasPpm) + " BATAS_BAWAH:" + String(batasBawahPpm));
      ppmTooLowOrHigh();
    }

    if (millis() - ppmMillis > intervalPpm & modePpmStr == "OTOMATIS" & isPpmNormal) {
      ppmMillis = millis();
      bool isPompaPpmUpOn = !digitalRead(RelayPompaPpmUpPin);
      bool isPompaPpmDownOn = !digitalRead(RelayPompaPpmDownPin);
      //isPpmLow?
      if (isPpmTooLow) {
        Serial.println("Ppm TOO LOW");
        if (sensPpm >= targetPpm) {
          limSendCalculatePpmInfo = false;
          aktivitasPpm = "atur ppm selesai";
          setAktifitas("ATUR PPM KE " + String(targetPpm, 0) + " SELESAI");
          Serial.println("ATUR PPM KE " + String(targetPpm, 0) + " SELESAI");
          isPpmMixingBegin = false;
          digitalWrite(RelayPompaPpmUpPin, !LOW);
          return;
        }
        if (isPompaPpmUpOn) {
          intervalPpm = intervalOffPpm;
          digitalWrite(RelayPompaPpmUpPin, !LOW);
        } else {
          intervalPpm = intervalOnPpm;
          digitalWrite(RelayPompaPpmUpPin, !HIGH);
        }
        digitalWrite(RelayPompaPpmDownPin, !LOW);
      }
      if (isPpmTooHigh) {
        Serial.println("Ppm TOO HIGH");
        if (sensPpm <= targetPpm) {
          limSendCalculatePpmInfo = false;
          aktivitasPpm = "atur ppm selesai";
          Serial.println("atur ppm KE " + String(targetPpm, 0) + " SELESAI");
          setAktifitas("atur ppm KE " + String(targetPpm, 0) + " SELESAI");
          isPpmMixingBegin = false;
          digitalWrite(RelayPompaPpmDownPin, !LOW);
          return;
        }
        if (isPompaPpmDownOn) {
          intervalPpm = intervalOffPpm;
          digitalWrite(RelayPompaPpmDownPin, !LOW);
        } else {
          intervalPpm = intervalOnPpm;
          digitalWrite(RelayPompaPpmDownPin, !HIGH);
        }
        digitalWrite(RelayPompaPpmUpPin, !LOW);
      }
    }
  }
}

void limitActivePenyiraman() {
  if (pompaPenyiramanStats) {
    if (pompaPenyiramanStats != lastStatsLimitPenyiraman) {
      lastStatsLimitPenyiraman = pompaPenyiramanStats;
      lastMillisPenyiraman = millis();
    }
    if (millis() - lastMillisPenyiraman > intervalLimit) {
      digitalWrite(RelayPompaPenyiramanPin, !LOW);
      pompaPenyiramanStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPenyiraman = millis();
  }
}

void limitActivePhUp() {
  if (pompaPhUpStats) {
    if (pompaPhUpStats != lastStatsLimitPhUp) {
      lastStatsLimitPhUp = pompaPhUpStats;
      lastMillisPhUp = millis();
    }
    if (millis() - lastMillisPhUp > intervalLimit) {
      digitalWrite(RelayPompaPhUpPin, !LOW);
      pompaPhUpStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPhUp = millis();
  }
}

void limitActivePhDown() {
  if (pompaPhDownStats) {
    if (pompaPhDownStats != lastStatsLimitPhDown) {
      lastStatsLimitPhDown = pompaPhDownStats;
      lastMillisPhDown = millis();
    }
    if (millis() - lastMillisPhDown > intervalLimit) {
      digitalWrite(RelayPompaPhDownPin, !LOW);
      pompaPhDownStats = false;
      Serial.println("limittt");
    }
  } else {
    lastMillisPhDown = millis();
  }
}

void limitActivePpmUp() {
  if (pompaPpmUpStats) {
    if (pompaPpmUpStats != lastStatsLimitPpmUp)
    {
      lastStatsLimitPpmUp = pompaPpmUpStats;
      lastMillisPpmUp = millis();
    }
    if (millis() - lastMillisPpmUp > intervalLimit)
    {
      digitalWrite(RelayPompaPpmUpPin, !LOW);
      pompaPpmUpStats = false;
      Serial.println("LIMITT");
    }
  } else {
    lastMillisPpmUp = millis();
  }
}

void limitActivePengisian() {
  if (pompaPengisianStats) {
    if (pompaPengisianStats != lastStatsLimitPengisian)
    {
      lastStatsLimitPengisian = pompaPengisianStats;
      lastMillisPengisian = millis();
    }
    if (millis() - lastMillisPengisian > intervalLimit)
    {
      digitalWrite(RelayPompaPengisianPin, !LOW);
      pompaPengisianStats = false;
      Serial.println("LIMITT");
    }
  } else {
    lastMillisPengisian = millis();
  }
}
void limitActiveSprayer() {
  if (sprayerStats) {
    if (sprayerStats != lastStatsLimitSprayer)
    {
      lastStatsLimitSprayer = sprayerStats;
      lastMillisSprayer = millis();
    }
    if (millis() - lastMillisSprayer > intervalLimit)
    {
      digitalWrite(RelaySprayerPin, !LOW);
      RelaySprayerPin = false;
      Serial.println("limit");
    }
  } else {
    lastMillisSprayer = millis();
  }
}

void limitAll() {
  limitActivePengisian();
  limitActivePpmUp();
  limitActivePhDown();
  limitActivePhUp();
  limitActivePenyiraman();
}

void tdsSetup()
{
  tds.setPin(TdsSensorPin);
  tds.TdsFilter(100);
  tds.setAref(3.3);  //reference voltage pada ADC, default 5.0V Pada Arduino UNO,MEGA,LEONARDO
  tds.setAdcRange(4096);  //1024 untuk 10bit ADC;4096 untuk 12bit ADC
  tds.useCalibration(1); //1 >> Menggunakan Mode Kalibrasi, 0 >> Tidak Menggunakan Mode Kalibrasi
  tds.begin();  //initialization
}

void readPpm() {
  tdsValue = tds.getTdsValue();  // Nilai TDS
  float _analogValue = tds.getAnalogValue(); // Nilai TDS
  float _getCompensationVolatge = tds.getCompensationVolatge();
  float _getKvalue = tds.getKvalue();
  if (!modeDummyPpm) {
    sensPpm = tdsValue;
    sensPpm = (0.68575 * sensPpm) + 87.44348;
  } else {
    sensPpm = dummyPpm;
  }
  sensPpm = (int)sensPpm;
}

void readTempWater() {
  sensorSuhu.requestTemperatures();
  sensTempWater = sensorSuhu.getTempCByIndex(0);
  if (sensTempWater <= 0 || sensTempWater > 100) sensTempWater = 25;
}

void readPh() {
  float voltAvg;
  for (byte i = 0; i < 100; i++) {
    voltAvg += analogRead(PH_PIN);
  }
  voltAvg = voltAvg / 100;
  voltage = voltAvg / 4096.0 * 3300;
  if (!modeDummyPh) {
    sensPh = ph.readPH(voltage, sensTempWater);
  } else {
    sensPh = dummyPh;
  }
  sensPh = roundToOneDecPoint(sensPh);
}

bool getResultMsg(ModbusMaster *node, uint8_t result)
{
  String tmpstr2 = "\r\n";
  switch (result)
  {
    case node->ku8MBSuccess:
      return true;
      break;
    case node->ku8MBIllegalFunction:
      tmpstr2 += "Illegal Function";
      break;
    case node->ku8MBIllegalDataAddress:
      tmpstr2 += "Illegal Data Address";
      break;
    case node->ku8MBIllegalDataValue:
      tmpstr2 += "Illegal Data Value";
      break;
    case node->ku8MBSlaveDeviceFailure:
      tmpstr2 += "Slave Device Failure";
      break;
    case node->ku8MBInvalidSlaveID:
      tmpstr2 += "Invalid Slave ID";
      break;
    case node->ku8MBInvalidFunction:
      tmpstr2 += "Invalid Function";
      break;
    case node->ku8MBResponseTimedOut:
      tmpstr2 += "Response Timed Out";
      break;
    case node->ku8MBInvalidCRC:
      tmpstr2 += "Invalid CRC";
      break;
    default:
      tmpstr2 += "Unknown error: " + String(result);
      break;
  }
  Serial.println(tmpstr2);
  return false;
}

void readFloat() {
  sensFloat = !digitalRead(floatSensorPin);
}

void readSensor() {
  readFloat();
  readTempWater();
  readPh();
  readPpm();
}
