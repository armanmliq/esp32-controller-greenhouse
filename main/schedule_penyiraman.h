
String listLamaPenyiraman = "";
String listEpochStartDatePenyiraman = "";     
String listEpochEndDatePenyiraman = "";
tmElements_t time_penyiraman;
time_t unix_start_penyiraman; 
time_t unix_end_penyiraman; 
void parseSchedulePenyiraman(String date, String _lamaPenyiraman) 
{
    Serial.println("TOD : " + date);
    int _day = 29;
    int _month = 5;
    
    int _year = 2022;
    int _hour = date.substring(10, 12).toInt();
    int _min = date.substring(13, 15).toInt();
    
    time_penyiraman.Second = 0;
    time_penyiraman.Hour = _hour;
    time_penyiraman.Minute = _min;
    time_penyiraman.Day = _day;
    time_penyiraman.Month = _month;      // months start from 0, so deduct 1
    time_penyiraman.Year = _year - 1970; // years since 1970, so deduct 1970
    unix_start_penyiraman =  makeTime(time_penyiraman);
    unix_end_penyiraman = unix_start_penyiraman + (_lamaPenyiraman.toInt() * 60);
    listEpochStartDatePenyiraman += String(unix_start_penyiraman)+',';
    listEpochEndDatePenyiraman += String(unix_end_penyiraman) +',';

}

//check given data in range
bool isInRangePenyiraman(String epochStartDateStr,String epochEndDateStr){
  unsigned long epochStartDate = epochStartDateStr.toInt();
   unsigned long epochEndDate = epochEndDateStr.toInt();
  if(epochNow < epochEndDate && epochNow > epochStartDate){
    return true;  
  }else{
    return false;
  }
}

//check if any aktif date, change target ppm
void CheckSchedulePenyiraman(){
  
    for(int i=0; i< _indexSchedulePenyiraman; i++){
    String epochStart= getValue(listEpochStartDatePenyiraman,',', i);
    String epochEnd= getValue(listEpochEndDatePenyiraman,',', i);
    Serial.println(String(i)+" : " + isInRangePenyiraman(epochStart, epochEnd));    
  }
}
