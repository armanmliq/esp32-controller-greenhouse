
//parsing separate comma
String getValue(String data, char separator, int index1)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index1; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index1 ? data.substring(strIndex[0], strIndex[1]) : "";
}

//count total specific char in a string
int count(String s, char c)
{
  int res = 0;
  for (int i = 0; i < s.length(); i++)
    if (s[i] == c)
      res++;
  return res;
}
