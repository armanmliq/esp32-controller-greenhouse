String statePump(bool _state) {
  if (_state ) {
    return "HIDUP";
  } else {
    return "MATI";
  }
}
//  updateSprayerStats = true;
//  updateTargetPpmStats = true;
//  updateWaterTempStats = true;
//  updateTempRoomStats = true;
//  updatePhStats
//  updatePpmStats = true;
//  updateHumidityStats = true;
//  updatePpmUpStats
//  updatePenyiramanStats
//  updatePengisianStats
//  updatePhUpStats
//  updatePhDownStats
void detectChangeSprayer() {
  sprayerStats = !digitalRead(RelaySprayerPin);
  if (sprayerStats != savedStatsSprayer) {
    savedStatsSprayer = sprayerStats;
    updateSprayerStats = true;
  }
}
void detectChangeTargetPpm() {
  if (targetPpm != savedStatsTargetPpm) {
    savedStatsTargetPpm = targetPpm;
    updateTargetPpmStats = true;
  }
}
void detectChangePenyiraman() {
  pompaPenyiramanStats = !digitalRead(RelayPompaPenyiramanPin);
  if (pompaPenyiramanStats != savedStatsPenyiraman) {
    savedStatsPenyiraman = pompaPenyiramanStats;
    updatePenyiramanStats = 1;
  }
}
void detectChangePengisian() {
  pompaPengisianStats = !digitalRead(RelayPompaPengisianPin);
  if (pompaPengisianStats != savedStatsPengisian) {
    savedStatsPengisian = pompaPengisianStats;
    updatePengisianStats = true;
  }
}
void detectChangePhUp() {
  pompaPhUpStats = !digitalRead(RelayPompaPhUpPin);
  if (pompaPhUpStats != savedStatsPhUp) {
    savedStatsPhUp = pompaPhUpStats;
    updatePhUpStats = true;
  }
}
void detectChangePhDown() {
  pompaPhDownStats = !digitalRead(RelayPompaPhDownPin);
  if (pompaPhDownStats != savedStatsPhDown) {
    savedStatsPhDown = pompaPhDownStats;
    updatePhDownStats = true;
  }
}
void detectChangePpmUp() {
  pompaPpmUpStats = !digitalRead(RelayPompaPpmUpPin);
  if (pompaPpmUpStats != savedStatsPpmUp) {
    savedStatsPpmUp = pompaPpmUpStats;
    updatePpmUpStats  = true;
  }
}
void detectChangePh() {
  if (sensPh != savedStatsPh) {
    savedStatsPh = sensPh;
    updatePhStats = true;
  }
}

void detectChangePpm() {
  if (sensPpm != savedStatsPpm) {
    savedStatsPpm = sensPpm;
    updatePpmStats = true;
  }
}
void detectChangeHumidity() {
  if (sensHumidity != savedStatsHumidity) {
    savedStatsHumidity = sensHumidity;
    updateHumidityStats = true;
  }
}
void detectChangeTempRoom() {
  if (sensTempRoom != savedStatsTempRoom) {
    savedStatsTempRoom = sensTempRoom;
    updateTempRoomStats  = true;
  }
}
void detectChangeWaterTemp() {
  if (sensTempWater != savedStatsWaterTemp) {
    savedStatsWaterTemp = sensTempWater;
    updateWaterTempStats = true;
  }
}
void sendStatsSprayer() {
  if (updateSprayerStats) {
    updateSprayerStats = false;
    Serial.println("UPDATE-SPRAYER");
    //    pushFirebase("sprayer", statePump(sprayerStats), "sensor_status");
    pushFirebase("set_sprayer", statePump(0), "set_parameter");
  }
}
void sendStatsTargetPpm() {
  if (updateTargetPpmStats) {
    Serial.println("UPDATE-TARGETPPM");
    pushFirebase("set_ppm", String(targetPpm, 0), "set_parameter");
    updateTargetPpmStats = false;
  }
}
void sendNotifJadwalPenyiraman() {
  if (dispPenyiramanState(dispPenyiramanStr)) {
    if (statePump(pompaPenyiramanStats) == "HIDUP") {
      title = "jadwal penyiraman hidup telah aktif";
      body = "ph:" + String(sensPh, 1) + "  ppm:" + String(sensPpm, 0);
      isSendNotif = true;
    }
  }
}

void sendNotifPengisian(String status) {
  delay(500);
  sensFloat = !digitalRead(floatSensorPin);
  if (status == "HIDUP") {
    if (sensFloat) {
      title = "AIR TANDON HABIS!";
      body = "Pompa pengisian hidup";
      isSendNotif = true;
    }
  } else {
    if (!sensFloat) {
      title = "WARNING!";
      body = "Pengisian Selesai";
      isSendNotif = true;
    }
  }
}

void sendStatsPenyiraman() {
  if (updatePenyiramanStats) {
    sendNotifJadwalPenyiraman();
    updatePenyiramanStats = false;
    String _status = statePump(pompaPenyiramanStats);
    updateGrafik("statusPompaPenyiraman", String(pompaPenyiramanStats));
    pushFirebase("set_pompa_penyiraman", _status,  "set_parameter");
    Serial.println("UPDATE-PENYIRAMAN " + _status);
    preferences.putString("initSiram", _status);
  }
}

void sendStatsPengisian() {
  if (updatePengisianStats) {
    updatePengisianStats = false;
    String _status = statePump(pompaPengisianStats);
    sendNotifPengisian(_status);
    updateGrafik("statusPompaPengisian", String(pompaPengisianStats));
    pushFirebase("set_pompa_pengisian", _status,  "set_parameter");
    Serial.println("UPDATE-PENGISIAN " + _status);
    preferences.putString("initPengisian", _status);
  }
}

void sendStatsPhUp() {

  if (updatePhUpStats) {
    Serial.println("UPDATE-PHUP");
    //    pushFirebase("pompaPhUpStatus", statePump(pompaPhUpStats),  "sensor_status");
    pushFirebase("set_pompa_ph_up", statePump(pompaPhUpStats), "set_parameter");
    updatePhUpStats = false;
  }
}
void sendStatsPhDown() {
  if (updatePhDownStats) {
    updatePhDownStats = false;
    //    pushFirebase("pompaPhDownStatus", statePump(pompaPhDownStats), "sensor_status");
    pushFirebase("set_pompa_ph_down", statePump(pompaPhDownStats), "set_parameter");
  }
}
void sendStatsPpmUp() {
  if (updatePpmUpStats)
  {
    updatePpmUpStats = false;
    //    pushFirebase("pompa_nutrisi_status", statePump(pompaPpmUpStats), "sensor_status");
    pushFirebase("set_pompa_ppm_up", statePump(pompaPpmUpStats), "set_parameter");
  }
}
void sendStatsPh() {
  if (updatePhStats) {
    updatePhStats = false;
    pushFirebase("ph", String(sensPh, 1), "sensor_status");
  }
}
void sendStatsPpm() {
  if (updatePpmStats) {
    updatePpmStats = false;
    pushFirebase("ppm", String(sensPpm, 0), "sensor_status");
  }
}

void sendStatsHumidity() {
  if (updateHumidityStats) {
    updateHumidityStats = false;
    pushFirebase("humidity", String(sensHumidity, 1), "sensor_status");
  }
}
void sendStatsWaterTemp() {
  if (updateWaterTempStats) {
    updateWaterTempStats = false;
    pushFirebase("temperatureWater", String(sensTempWater, 1), "sensor_status");
  }
}
void sendStatsTempRoom() {
  if (updateTempRoomStats) {
    updateTempRoomStats = false;
    pushFirebase("temperature", String(sensTempRoom, 1), "sensor_status");
  }
}
void sendStatsAktifitas() {
  if (updateAktifitasStats) {
    updateAktifitasStats = false;
    updateGrafik("aktifitas", aktifitasStr);
  }
}
void sendAllPumpStats() {
  updatePenyiramanStats, updatePhUpStats, updatePhDownStats, updatePpmUpStats, updatePengisianStats, updateTargetPpmStats, updateSprayerStats = true;
}
void sendAllSensorStats() {
  updateAktifitasStats, updateTempRoomStats, updateWaterTempStats, updateHumidityStats, updatePpmStats, updatePhStats = true;
}


void sendAllStats() {
  sendStatsPh();
  sendStatsPpm();
  sendStatsHumidity();
  sendStatsWaterTemp();
  sendStatsTempRoom();
  sendStatsTargetPpm();
  sendStatsAktifitas();
  sendStatsSprayer();
  sendStatsPenyiraman();
  sendStatsPengisian();
  sendStatsPhUp();
  sendStatsPhDown();
  sendStatsPpmUp();
}

void detectChangeAllSensor() {
  //  detectChangeSprayer();
  //  detectChangePenyiraman();
  //  detectChangePengisian();
  //  detectChangePhUp();
  //  detectChangePhDown();
  //  detectChangePpmUp();

  detectChangeTargetPpm();
  detectChangePh();
  detectChangePpm();
  detectChangeHumidity();
  detectChangeTempRoom();
  detectChangeWaterTemp();
}
