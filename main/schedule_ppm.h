#include "TimeLib.h"

tmElements_t time_ppm;  // time elements structure
time_t unix_start_ppm; // a timestamp
//comma separated 
String listEpochStartPpm;
String listEpochEndPpm;
String listPpm;
long epochNow = 1653857164;


//parsing start date
void parseDateStart(String date) 
{  
  int _day = date.substring(8, 10).toInt();
  int _month = date.substring(5, 7).toInt();
  int _year = date.substring(0, 4).toInt();
  time_ppm.Second = 0;
  time_ppm.Hour = 0;
  time_ppm.Minute = 0;
  time_ppm.Day = _day;
  time_ppm.Month = _month;      // months start from 0, so deduct 1
  time_ppm.Year = _year - 1970; // years since 1970, so deduct 1970
  unix_start_ppm =  makeTime(time_ppm);
  listEpochStartPpm += String(unix_start_ppm)+',';
}


//parsing end date
void parseDateEnd(String date) 
{  
  int _day = date.substring(8, 10).toInt();
  int _month = date.substring(5, 7).toInt();
  int _year = date.substring(0, 4).toInt();
  time_ppm.Second = 0;
  time_ppm.Hour = 0;
  time_ppm.Minute = 0;
  time_ppm.Day = _day;
  time_ppm.Month = _month;      // months start from 0, so deduct 1
  time_ppm.Year = _year - 1970; // years since 1970, so deduct 1970
  unix_start_ppm =  makeTime(time_ppm);
  listEpochEndPpm += String(unix_start_ppm)+',';
}


//parsing ppm
void parsePpm(String ppm) 
{  
  listPpm += String(ppm)+',';
}

//check given data in range
bool isInRangePpm(String epochStartDateStr,String epochEndDateStr, String ppm){
  unsigned long epochStartDate = epochStartDateStr.toInt();
   unsigned long epochEndDate = epochEndDateStr.toInt();
  if(epochNow < epochEndDate && epochNow > epochStartDate){
    return true;  
  }else{
    return false;
  }
}

//check if any aktif date, change target ppm
void CheckSchedulePpm(){
    for(int i=0; i< _indexSchedulePpm; i++){
    String epochStart= getValue(listEpochStartPpm,',', i);
    String epochEnd= getValue(listEpochEndPpm,',', i);
    String _ppm = getValue(listPpm,',', i);
    
   if(isInRangePpm(epochStart, epochEnd, _ppm)){
    targetPpm = _ppm.toFloat();
   }
    
    Serial.println(String(i)+" inrange?: " + isInRangePpm(epochStart, epochEnd, _ppm));    
    Serial.println(String(i)+" " +String(_ppm));
  }
}
