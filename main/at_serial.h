void serial() {
  if (Serial.available() > 0)
  {
    char dat = Serial.read();
    if (dat == 'a')
    {
      updateAllGrafik();
      Serial.println("response");
    }

    if (dat == 'd')
    {
      Serial.println("delete proccess");
      coreDeleteGrafik();
    }

    if (dat == 'p') {
      int _ppm = Serial.parseInt();
      pushFirebase("set_ppm", String(_ppm),"/set_parameter");
    }
  }
}
