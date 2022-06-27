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
bool isPhInRange() {
  if (sensPh > 0 & sensPh < 12) {
    return true;
  } else {
    return false;
  }
}
bool isPpmInRange() {
  if (sensPpm > 0 & sensPpm < 1300) {
    return true;
  } else {
    return false;
  }
}
void setupForPhMixing()
{
  if (!isPhInRange) {
    setAktifitas("WARNING! PH MASIH DALAM SET POINT AREA! SKIP ADJUSTMENT");
    return;
  }
  setAktifitas("WARNING! PH DILUAR BATAS AREA");
  Serial.println("WARNING! PH DILUAR BATAS AREA \n");
  isPhTooHigh = false;
  isPhTooLow = false;
  phMixingExcBeginning = false;
  isPpmMixingBegin = false;
  isPhMixingBegin = true;
}
void setupForPpm() {
  if (!isPpmInRange) {
    setAktifitas("WARNING! NUTRISI PPM MASIH DALAM AREA SET POINT, SKIP ADJUSTMENT!");
    Serial.println("WARNING! PPM IS IN TARGET AREA.. SKIPPING PROCESS PPM!");
    return;
  }
  setAktifitas("WARNING! PPM DILUAR BATAS SET POINT..  PPM ADJUSTMENT DI PROCESS..");
  Serial.println("WARNING! PPM DILUAR BATAS TARGET..  ABMIX ADJUST PROCESS...");
  isPpmMixingBegin = true;
  intervalPpm = intervalOnPpm;
  dispActivity("adjust nutrition abmix");
}
void detectForFilling() {
  if (sensFloat) {
    Serial.println("WARNING! AIR TANDON HABIS.. POMPA TANDON DIHIDUPKAN..");
    setAktifitas("WARNING! AIR TANDON HABIS.. POMPA TANDON DIHIDUPKAN..");
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
  if (isFloatTrue & !sensFloat & !isFloatLimit) {
    Serial.println("WARNING! PENGISIAN TANDON SELESAI");
    setAktifitas("WARNING! PENGISIAN TANDON SELESAI, POMPA TANDON DIMATIKAN");
    delay(500);
    isFloatLimit = true;
    digitalWrite(RelayPompaPengisianPin, LOW);
    if (modePhStr == "OTOMATIS") {
      setupForPhMixing();
    }
  }
}
void detectForPhMixing() {
  if (isPhMixingBegin) {
    //HINDARI EKSEKUSI PH PADA:
    //MODE MANUAL
    if (modePhStr != "OTOMATIS") {
      Serial.println("WARNING! PROSES PH DIHINDARI PADA MODE MANUAL");
      setAktifitas("WARNING! PROSES PH DIHINDARI PADA MODE MANUAL");
    }
    //PENGISIAN BERJALAN
    if (sensFloat) {
      setAktifitas("WARNING! PROSES PH PADA SAAT PENGISIAN");
      Serial.println("WARNING! MENGHINDARI PROSES PH PADA SAAT PENGISIAN");
    }
    if (modePhStr != "OTOMATIS" || sensFloat) {
      return;
    }

    if (!phMixingExcBeginning) {
      phMixingExcBeginning = true;
      intervalPh = intervalOnPh;
      batasAtasPh = targetPh + batasMarginPh;
      batasBawahPh = targetPh - batasMarginPh;
      setAktifitas("WARNING! calculating PH -> TARGET:" + String(targetPh) + " PH-ATAS:" + String(batasAtasPh) + " PH-BAWAH:" + String(batasBawahPh));
      Serial.println("WARNING! calculating PH -> TARGET:" + String(targetPh) + " PH-ATAS:" + String(batasAtasPh) + " PH-BAWAH:" + String(batasBawahPh));
      if (sensPh > batasAtasPh) {
        isPhTooHigh = true;
        isPhTooLow = false;
        Serial.println("WARNING! PH TERLALU TINGGI, PROCESS MENURUNKAN.. " + String(sensPh) + " ==> (TARGET)" + String(targetPh));
        setAktifitas("WARNING! PH TERLALU TINGGI, PROCESS MENURUNKAN.. " + String(sensPh) + " ==> (TARGET)" + String(targetPh));
        dispActivity("PH " + String(sensPh) + " ==> " + String(targetPh));
      }
      if (sensPh < batasBawahPh) {
        isPhTooLow = true;
        isPhTooHigh = false;
        setAktifitas("WARNING! PH TERLALU RENDAH, PROCESS MENAIKKAN.. " + String(sensPh) + " ==> (TARGET)" + String(targetPh));
        Serial.println("WARNING! PH TERLALU RENDAH, PROCESS MENAIKKAN.. " + String(sensPh) + " ==> (TARGET)" + String(targetPh));
        dispActivity("PH " + String(sensPh) + " ==> " + String(targetPh));
      }
    }
    if (millis() - phMillis > intervalPh) {
      phMillis = millis();
      bool isPompaPhUpOn = digitalRead(RelayPompaPhUpPin);
      bool isPompaPhDownOn = digitalRead(RelayPompaPhDownPin);
      //isPhLow?
      if (isPhTooLow) {
        if (sensPh >= targetPh) {
          setAktifitas("WARNING! PROSES PH KE TARGET " + String(targetPh) + " SELESAI");
          Serial.println("WARNING! ADJUST PH TO " + String(targetPh) + " DONE");
          dispActivity("PH ADJUST DONE");
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
          Serial.println("WARNING! PROSES PH KE TARGET " + String(targetPh) + " SELESAI");
          setAktifitas("WARNING! ADJUST PH TO " + String(targetPh) + " SELESAI");
          dispActivity("PH ADJUST DONE");
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
bool savedStateAktifitas;
void detectForPpmMixing() {
  if (isPpmMixingBegin) {
    if (!savedStateAktifitas) {
      Serial.println("WARNING! MENGHINDARI PROSES PPM PADA MODE MANUAL");
      if (modePpmStr != "OTOMATIS") {
        setAktifitas("WARNING! MENGHINDARI PROSES PPM PADA MODE MANUAL");
      }
      if (sensFloat) {
        setAktifitas("WARNING! MENGHINDARI PROSES PPM PADA SAAT PENGISIAN");
        Serial.println("WARNING! MENGHINDARI PROSES PPM PADA SAAT PENGISIAN");
      }
    }

    if (modePpmStr != "OTOMATIS" || sensFloat) {
      savedStateAktifitas = 1;
      return;
    }
    savedStateAktifitas = 0;
    if (sensPpm > targetPpm) {
      digitalWrite(RelayPompaPpmUpPin, LOW);
      isPpmMixingBegin = false;
      setAktifitas("WARNING! PPM TERLALU TINGGI, MENURUNKAN PPM" + String(sensPpm) + " ==> (TARGET)" + String(targetPpm));
      Serial.println("WARNING! PPM TERLALU TINGGI, MENURUNKAN PPM" + String(sensPpm) + " == > (TARGET)" + String(targetPpm));
      dispActivity("PPM TOO HIGH " + String(sensPpm) + " == > " + String(targetPpm));
      return;
    }
    if (sensPpm < targetPpm) {
      setAktifitas("WARNING! PPM TERLALU RENDAH, MENAIKKAN PPM" + String(sensPpm) + " == > (TARGET)" + String(targetPpm));
      Serial.println("WARNING! PPM TERLALU RENDAH, MENAIKKAN PPM" + String(sensPpm) + " == > (TARGET)" + String(targetPpm));
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
void limitActivePenyiraman() {
  if (pompaPenyiramanStats) {
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
    if (pompaPpmUpStats != lastStatsLimitPpmUp)
    {
      lastStatsLimitPpmUp = pompaPpmUpStats;
      lastMillisPpmUp = millis();
    }
    if (millis() - lastMillisPpmUp > intervalLimit)
    {
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
    if (millis() - lastMillisPengisian > intervalLimit)
    {
      digitalWrite(RelayPompaPengisianPin, LOW);
      pompaPengisianStats = false;
      Serial.println("limit");
    }
  } else {
    lastMillisPengisian = millis();
  }
}
void limitActiveSprayer() {
  if (sprayerStats) {
    if (sprayerStats != lastStatsLimitSprayer) {
      lastStatsLimitSprayer = sprayerStats;
      lastMillisSprayer = millis();
    }
    if (millis() - lastMillisSprayer > intervalLimit)
    {
      digitalWrite(RelaySprayerPin, LOW);
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
  sensPpm = tdsValue;
}

void readTempWater() {
  sensorSuhu.requestTemperatures();
  sensTempWater = sensorSuhu.getTempCByIndex(0);
  if(sensTempWater <= 0 || sensTempWater > 100) sensTempWater = 25;
}

void readPh() {
  float voltAvg;
  for (byte i = 0; i < 100; i++) {
    voltAvg += analogRead(PH_PIN);
  }
  voltAvg = voltAvg / 100;
  voltage = voltAvg / 4096.0 * 3300;
  sensPh = ph.readPH(voltage, sensTempWater);
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

void readSHT20(void * parameter ) {
  for (;;) {
    if (readSHT) {
      readSHT = false;
      uint8_t result = modbus.readInputRegisters(0x01, 2);
      if (getResultMsg(&modbus, result))
      {
        double res_dbl = modbus.getResponseBuffer(0) / 10;
        sensTempRoom = res_dbl;
        res_dbl = modbus.getResponseBuffer(1) / 10;
        sensHumidity = res_dbl;
        if (sensTempRoom <= 0 || sensTempRoom >= 100) sensTempRoom = 25;
        if (sensHumidity <= 0 || sensHumidity > 100) sensHumidity = 80;
      }
    }
  }
  vTaskDelete( NULL );
}
void readFloat() {
  //  sensFloat = digitalRead(floatSensorPin);
  //  detectForFilling();
}
//updateGrafik("aktifitas", "WARNING! PH MASIH DALAM \n    BATAS AREA! SKIP ADJUSTMENT");
//updateGrafik("aktifitas", "WARNING! PH MASIH DALAM \n    BATAS AREA! SKIP ADJUSTMENT");
void readSensor() {
  readFloat();
  readTempWater();
  readPh();
  readPpm();
  readSHT = true;
}
