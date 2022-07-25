byte indexSchedulePenyiraman = 0;
String listLamaPenyiraman = "";
String listEpochStartDatePenyiraman = "";
String listEpochEndDatePenyiraman = "";
String dispPenyiramanStr;
bool offPenyiramanOne;
tmElements_t time_penyiraman;
time_t unix_start_penyiraman;
time_t unix_end_penyiraman;

bool dispPenyiramanState(String strState) 
{
  if (strState.indexOf('1') != -1) {
    return 1;
  } else if (strState.length() <= 0 ) {
    return 0;
  } else {
    return 0;
  }
}

void parseSchedulePenyiraman(String date, String _lamaPenyiraman)
{
  int _day = globalDay;
  int _month = globalMonth;
  int _year = globalYear;
  int _hour = date.substring(10, 12).toInt();
  int _min = date.substring(13, 15).toInt();
  time_penyiraman.Second = 0;
  time_penyiraman.Hour = _hour;
  time_penyiraman.Minute = _min;
  time_penyiraman.Day = _day;
  time_penyiraman.Month = _month;      // months start from 0, so deduct 1
  time_penyiraman.Year = _year - 1970; // years since 1970, so deduct 1970
  unix_start_penyiraman =  makeTime(time_penyiraman);
  unix_end_penyiraman = unix_start_penyiraman + _lamaPenyiraman.toInt();
  listEpochStartDatePenyiraman += String(unix_start_penyiraman) + ',';
  listEpochEndDatePenyiraman += String(unix_end_penyiraman) + ',';
}

//check given data in range
bool isInRangePenyiraman(String epochStartDateStr, String epochEndDateStr) {
  unsigned long epochStartDate = epochStartDateStr.toInt();
  unsigned long epochEndDate = epochEndDateStr.toInt();
  if (globalEpoch < epochEndDate && globalEpoch > epochStartDate) {
    return true;
  } else {
    return false;
  }
}
//check if any aktif date, change target ppm
void CheckSchedulePenyiraman()
{
  dispPenyiramanStr = "";
  for (int i = 0; i < indexSchedulePenyiraman; i++) {
    String epochStart = getValue(listEpochStartDatePenyiraman, ',', i);
    String epochEnd = getValue(listEpochEndDatePenyiraman, ',', i);
    dispPenyiramanStr += isInRangePenyiraman(epochStart, epochEnd);
  }

  //check string if contain char "1"
  if (dispPenyiramanState(dispPenyiramanStr)) {
    offPenyiramanOne = false;
    digitalWrite(RelayPompaPenyiramanPin, !HIGH);
    digitalWrite(RelayValvePenyiramanPin, !HIGH);
    
  }

    //check string if not contain char "1"
  if (!offPenyiramanOne && !dispPenyiramanState(dispPenyiramanStr)) {
    offPenyiramanOne = true;
    digitalWrite(RelayPompaPenyiramanPin, !LOW);
    digitalWrite(RelayValvePenyiramanPin, !LOW);
  }

}
