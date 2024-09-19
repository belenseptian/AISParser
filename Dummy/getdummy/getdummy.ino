#include <AIS.h>

#define RXD2 16
#define TXD2 17
/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )

void printDegrees(long min4) 
{
//    Serial.print(min4); Serial.print(" (");Serial.print(min4,16);Serial.println(")");
    long intPart = min4 / 60L;
    long fracPart = intPart % 10000L;
    if (fracPart < 0)
      fracPart = -fracPart;
    char frac[6];
    sprintf(frac, "%04ld", fracPart);
    Serial.print(intPart/10000L);Serial.print(".");Serial.print(frac);
}

void showMMSI(AIS& ais_msg) {
  unsigned long mmsi = ais_msg.get_mmsi();
  Serial.print("Returned MMSI: ");
  Serial.print(mmsi);
  Serial.print(" ("); Serial.print(mmsi, 16); Serial.print(" )");
  Serial.println("");
}

void showSOG(AIS& ais_msg) {
    unsigned int SOG = ais_msg.get_SOG();
    Serial.print("Returned SOG: ");
    Serial.print( (SOG) / 10 ); Serial.print("."); Serial.print( (SOG) % 10 ); Serial.println(" nm");  
}

void showCOG(AIS& ais_msg) {
    unsigned int COG = ais_msg.get_COG();
    Serial.print("Returned COG: ");
    Serial.print( (COG) / 10 ); Serial.print("."); Serial.print( (COG) % 10 ); Serial.println(" degrees");  
}

void showLatitude(AIS& ais_msg) {
    long LAT = ais_msg.get_latitude();
    Serial.print("Returned LAT: "); printDegrees(LAT); Serial.println(" degrees");  
}

void showLongitude(AIS& ais_msg) {
    long LONG = ais_msg.get_longitude();
    Serial.print("Returned LONG: "); printDegrees(LONG); Serial.println(" degrees");  
}

int loA(char *ptr)
{
    int sample = 0;

    while (*ptr != '\0')
    {
        sample++;
        ptr++;
    }
    return sample;
}

String getSentence()
{
  String ais;
  if(Serial2.available()>0) {
    ais = Serial2.readStringUntil('!');
  }
  return ais;
}

char * converttoChars(String sentence)
{
  char * buf = (char *) malloc (100);
  sentence.toCharArray(buf,sentence.length());
  return buf;
}

char * getDynamicMessageA(char * rawAIS)
{
  char * ais = rawAIS;
  static char out[1001];
  int store_index = 13;
  int length = loA(ais);
  for (int i = 0; i < length + 2; i++)
  {
    char a_s = ais[store_index];
    out[i] = a_s;
    store_index++;
  }
  return out;
}

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void loop() { //Choose Serial1 or Serial2 as required
  // Serial.print(getSentence());
  char * rawAIS = converttoChars(getSentence());
  char * msg = getDynamicMessageA(rawAIS);
  char isStatic = rawAIS[8];
  char msgType = rawAIS[13];
  // Serial.println(msgType);
  // Serial.println(rawAIS);
  // Serial.println(msg);
  // Serial.println(isStatic);
  // AIS ais_msg(msg);
  // showMMSI(ais_msg);
  // showSOG(ais_msg);
  // showCOG(ais_msg);
  // showLatitude(ais_msg);
  // showLongitude(ais_msg);
  free(rawAIS);
  delay(10);
}