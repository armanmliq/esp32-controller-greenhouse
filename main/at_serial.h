void serial() {
  if (Serial.available() > 0)
  {
    char dat = Serial.read();
    if (dat == 'p')
    {
      float ph = Serial.parseFloat();
      sensPh = ph;
      Serial.println("set sensPH " + String(ph));
    }
    if (dat == 'm')
    {
      float ppm = Serial.parseInt();
      sensPpm = ppm;
      Serial.println("set sensPpm " + String(ppm));
    }
    if (dat == 'f')
    {
      sensFloat = !sensFloat;
      Serial.println("set sens_float " + String(sensFloat));
    }
    if (dat == 'x') {
      coreDeleteGrafik();
    }
    if (dat == 'a') {
      setAktifitas("TEST COMMAND FROM SERIAL");
    }
    if (dat == 'b') {
      setAktifitas("POMPA PENYIRAMAN DIHIDUPKAN");
    }
  }
}
