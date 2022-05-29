#include "TimeLib.h"

tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp
//comma separated 
String listStartDate;
String listEndDate;
String listPpm;
long epochNow = 1653784400;
//parsing start date
void parseDateStart(String date) 
{  
  int _day = date.substring(8, 10).toInt();
  int _month = date.substring(5, 7).toInt();
  int _year = date.substring(0, 4).toInt();
  my_time.Second = 0;
  my_time.Hour = 0;
  my_time.Minute = 0;
  my_time.Day = _day;
  my_time.Month = _month;      // months start from 0, so deduct 1
  my_time.Year = _year - 1970; // years since 1970, so deduct 1970
  unix_timestamp =  makeTime(my_time);
  listStartDate += String(unix_timestamp)+',';
}


//parsing end date
void parseDateEnd(String date) 
{  
  int _day = date.substring(8, 10).toInt();
  int _month = date.substring(5, 7).toInt();
  int _year = date.substring(0, 4).toInt();
  my_time.Second = 0;
  my_time.Hour = 0;
  my_time.Minute = 0;
  my_time.Day = _day;
  my_time.Month = _month;      // months start from 0, so deduct 1
  my_time.Year = _year - 1970; // years since 1970, so deduct 1970
  unix_timestamp =  makeTime(my_time);
  listEndDate += String(unix_timestamp)+',';
}


//parsing ppm
void parsePpm(String ppm) 
{  
  listPpm += String(ppm)+',';
}


//parsing separate comma
String getValue(String data, char separator, int index1)
{
//  Serial.println("data=" + data);
//  Serial.println("index=" + String(index1));
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
 
  for(int i=0; i<=maxIndex && found<=index1; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
  
  return found>index1 ? data.substring(strIndex[0], strIndex[1]) : "";
}

//count total specific char in a string
int count(String s, char c)
{
    int res = 0;
    for (int i=0;i<s.length();i++)
        if (s[i] == c)
            res++;
    return res;
}

//check given data in range
bool isInRange(String epochStartDateStr,String epochEndDateStr, String ppm){
  Serial.println("try isInrange " + ppm);
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
    String epochStart= getValue(listStartDate,',', i);
    String epochEnd= getValue(listEndDate,',', i);
    String _ppm = getValue(listPpm,',', i);
    
   if(isInRange(epochStart, epochEnd, _ppm)){
    targetPpm = _ppm.toFloat();
   }
    
    Serial.println(String(i)+" : " + isInRange(epochStart, epochEnd, _ppm));    
    Serial.println(String(i)+" " +String(_ppm));
  }
}
