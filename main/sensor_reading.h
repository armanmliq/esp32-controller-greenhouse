
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
}
