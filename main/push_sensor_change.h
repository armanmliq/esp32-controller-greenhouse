String statePump(bool _state) {
  _state ? "DIHIDUPKAN" : "DIMATIKAN";
}
void detectChangeSprayer() {
  sprayerStats = digitalRead(RelaySprayerPin);
  if (sprayerStats != savedStatsSprayer) {
    savedStatsSprayer = sprayerStats;
    updateSprayerStats = 1;
  }
}
void detectChangeTargetPpm() {
  if (targetPpm != savedStatsTargetPpm) {
    savedStatsTargetPpm = targetPpm;
    updateTargetPpmStats = 1;
  }
}
void detectChangePenyiraman() {
  pompaPenyiramanStats = digitalRead(RelayPompaPenyiramanPin);
  if (pompaPenyiramanStats != savedStatsPenyiraman) {
    savedStatsPenyiraman = pompaPenyiramanStats;
    updatePenyiramanStats = 1;
  }
}
void detectChangePengisian() {
  pompaPengisianStats = digitalRead(RelayPompaPengisianPin);
  if (pompaPengisianStats != savedStatsPengisian) {
    savedStatsPengisian = pompaPengisianStats;
    updatePengisianStats = 1;
  }
}
void detectChangePhUp() {
  pompaPhUpStats = digitalRead(RelayPompaPhUpPin);
  if (pompaPhUpStats != savedStatsPhUp) {
    savedStatsPhUp = pompaPhUpStats;
    updatePhUpStats = 1;
  }
}
void detectChangePhDown() {
  pompaPhDownStats = digitalRead(RelayPompaPhDownPin);
  if (pompaPhDownStats != savedStatsPhDown) {
    savedStatsPhDown = pompaPhDownStats;
    updatePhDownStats = 1;
  }
}
void detectChangePpmUp() {
  pompaPpmUpStats = digitalRead(RelayPompaPpmUpPin);
  if (pompaPpmUpStats != savedStatsPpmUp) {
    savedStatsPpmUp = pompaPpmUpStats;
    updatePpmUpStats = 1;
  }
}
void detectChangePh() {
  if (sensPh != savedStatsPh) {
    savedStatsPh = sensPh;
    updatePhStats = 1;
  }
}

void detectChangePpm() {
  if (sensPh != savedStatsPpm) {
    savedStatsPpm = sensPpm;
    updatePpmStats = 1;
  }
}
void detectChangeHumidity() {
  if (sensHumidity != savedStatsHumidity) {
    savedStatsHumidity = sensHumidity;
    updateHumidityStats = 1;
  }
}
void detectChangeTempRoom() {
  if (sensTempRoom != savedStatsTempRoom) {
    savedStatsTempRoom = sensTempRoom;
    updateTempRoomStats = 1;
  }
}

void detectChangeWaterTemp() {
  if (sensTempWater != savedStatsWaterTemp) {
    savedStatsWaterTemp = sensTempWater;
    updateWaterTempStats = 1;
  }
}
void sendStatsSprayer() {
  if (updateSprayerStats) {
    updateSprayerStats = false;
    String _sprayerStats =  sprayerStats ? "HIDUP" : "MATI";
    pushFirebase("sprayer", _sprayerStats, "sensor_status");
    pushFirebase("set_sprayer", _sprayerStats, "set_parameter");
  }
}
void sendStatsTargetPpm() {
  if (updateTargetPpmStats) {
    pushFirebase("set_ppm", String(targetPpm, 0), "set_parameter");
    updateTargetPpmStats = 0;
  }
}
void sendStatsPenyiraman() {
  if (updatePenyiramanStats) {
    updatePenyiramanStats = false;
    String _pompaStats =  pompaPenyiramanStats ? "HIDUP" : "MATI";
    updateGrafik("statusPompaPenyiraman", String(pompaPenyiramanStats));
    pushFirebase("pompaPenyiraman", _pompaStats, "sensor_status");
    pushFirebase("set_pompa_penyiraman", _pompaStats,  "set_parameter");
  }
}
void sendStatsPengisian() {
  if (updatePengisianStats) {
    updatePengisianStats = false;
    String _pompaStats =  pompaPengisianStats ? "HIDUP" : "MATI";
    pushFirebase("pompa_status", _pompaStats, "sensor_status");
    pushFirebase("set_pompa_pengisian", _pompaStats, "set_parameter");
  }
}
void sendStatsPhUp() {
  if (updatePhUpStats) {
    String _pompaStats =  pompaPhUpStats ? "HIDUP" : "MATI";
    pushFirebase("pompaPhUpStatus", _pompaStats,  "sensor_status");
    pushFirebase("set_pompa_ph_up", _pompaStats, "set_parameter");
    updatePhUpStats = false;
  }
}

void sendStatsPhDown() {
  if (updatePhDownStats) {
    updatePhDownStats = false;
    String _pompaStats =  pompaPhDownStats ? "HIDUP" : "MATI";
    pushFirebase("pompaPhDownStatus", _pompaStats, "sensor_status");
    pushFirebase("set_pompa_ph_down", _pompaStats, "set_parameter");
  }
}
void sendStatsPpmUp() {
  if (updatePpmUpStats)
  {
    String _pompaStats =  pompaPpmUpStats ? "HIDUP" : "MATI";
    pushFirebase("pompa_nutrisi_status", _pompaStats, "sensor_status");
    pushFirebase("set_pompa_ppm_up", _pompaStats, "set_parameter");
    updatePpmUpStats = false;
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

void sendAllStats() {
  sendStatsAktifitas();
  sendStatsSprayer();
  sendStatsTargetPpm();
  sendStatsPenyiraman();
  sendStatsPengisian();
  sendStatsPhUp();
  sendStatsPhDown();
  sendStatsPpmUp();
  sendStatsPh();
  sendStatsPpm();
  sendStatsHumidity();
  sendStatsWaterTemp();
  sendStatsTempRoom();
}
void detectChangeAllSensor() {
  detectChangeSprayer();
  detectChangeTargetPpm();
  detectChangePenyiraman();
  detectChangePengisian();
  detectChangePhUp();
  detectChangePhDown();
  detectChangePpmUp();
  detectChangePh();
  detectChangePpm();
  detectChangeHumidity();
  detectChangeTempRoom();
  detectChangeWaterTemp();
}
