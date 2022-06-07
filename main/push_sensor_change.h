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
  if (pompaPpmUpStats != savedStatsPhDown) {
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
  if (sensWaterTemp != savedStatsWaterTemp) {
    savedStatsWaterTemp = sensWaterTemp;
    updateWaterTempStats = 1;
  }
}

void sendStatsPenyiraman() {
  if (updatePenyiramanStats) {
    updatePenyiramanStats = false;
    updateGrafik("statusPompaPenyiraman", String(pompaPenyiramanStats));
    updateStatusPompa("pompaPenyiraman", pompaPenyiramanStats);
  }
}
void sendStatsPengisian() {
  if (updatePengisianStats) {
    updatePengisianStats = false;
    updateStatusPompa("pompa_status", pompaPengisianStats);
  }
}
void sendStatsPhUp() {
  if (updatePhUpStats) {
    updatePhUpStats = false;
    updateStatusPompa("pompaPhUpStatus", pompaPhUpStats);
  }
}

void sendStatsPhDown() {
  if (updatePhDownStats) {
    updatePhDownStats = false;
    updateStatusPompa("pompaPhDownStatus", pompaPhDownStats);
  }
}
void sendStatsPpmUp() {
  if (updatePhDownStats) {
    updatePpmUpStats = false;
    updateStatusPompa("pompa_nutrisi_status", pompaPpmUpStats);
  }
}
void sendStatsPh() {
  if (updatePhStats) {
    updatePhStats = false;
    updateStatusSensor("ph", String(sensPh, 1));
  }
}
void sendStatsPpm() {
  if (updatePpmStats) {
    updatePpmStats = false;
    updateStatusSensor("ppm", String(sensPpm, 0));
  }
}

void sendStatsHumidity() {
  if (updateHumidityStats) {
    updateHumidityStats = false;
    updateStatusSensor("humidity", String(sensHumidity, 1));
  }
}
void sendStatsWaterTemp() {
  if (updateWaterTempStats) {
    updateWaterTempStats = false;
    updateStatusSensor("temperatureWater", String(sensWaterTemp, 1));
  }
}
void sendStatsTempRoom() {
  if (updateTempRoomStats) {
    updateTempRoomStats = false;
    updateStatusSensor("temperature", String(sensTempRoom, 1));
  }
}


void sendAllStats() {
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
