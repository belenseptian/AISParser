#include <AIS.h>

#define RXD2 16
#define TXD2 17
/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )

const char * destination;
const char * name;
String lon;
String lat;
String sog;
String cog;
unsigned int hdg;
unsigned int timeS;
long mmsi;
long imo;
String eta;
String draught;
int shiptype;


// ========Begin Dynamic Data========
String showLongitude(AIS& ais_msg) 
{
  long in = ais_msg.get_longitude();
  long intPart = in / 60L;
  long fracPart = intPart % 10000L;
  if (fracPart < 0)
    fracPart = -fracPart;
  char frac[6];
  sprintf(frac, "%04ld", fracPart);
  // Serial.print(intPart/10000L);Serial.print(".");Serial.print(frac);
  String integerpart = String(intPart/10000L);
  String decimalpart = String(frac);
  String out = integerpart+"."+decimalpart;

  return out;
}

String showLatitude(AIS& ais_msg) 
{
  long in = ais_msg.get_latitude();
  long intPart = in / 60L;
  long fracPart = intPart % 10000L;
  if (fracPart < 0)
    fracPart = -fracPart;
  char frac[6];
  sprintf(frac, "%04ld", fracPart);
  // Serial.print(intPart/10000L);Serial.print(".");Serial.print(frac);
  String integerpart = String(intPart/10000L);
  String decimalpart = String(frac);
  String out = integerpart+"."+decimalpart;

  return out;
}

String showSOG(AIS& ais_msg) {
  unsigned int in = ais_msg.get_SOG();
  String integerpart = String((in) / 10);
  String decimalpart = String((in) % 10);
  String out = integerpart+"."+decimalpart;

  return out;
}

String showCOG(AIS& ais_msg) {
  unsigned int in = ais_msg.get_COG();
  String integerpart = String((in) / 10);
  String decimalpart = String((in) % 10);
  String out = integerpart+"."+decimalpart;

  return out;
}

unsigned int heading(AIS& ais_msg) {
  unsigned int hdg = ais_msg.get_HDG();
  return hdg;
}

unsigned int timestamp(AIS& ais_msg) {
  unsigned int time = ais_msg.get_timeStamp();
  return time;
}
// ========End Dynamic Data========

// ========Begin Static Data========
String getETA(AIS& ais_msg)
{
  unsigned int month = ais_msg.get_month();
  unsigned int day = ais_msg.get_day();
  unsigned int hour = ais_msg.get_hour();
  unsigned int minute = ais_msg.get_minute(); 
  String out = String(month)+"-"+String(day)+"-"+String(hour)+"-"+String(minute);

  return out;
}

const char * getDestination(AIS& ais_msg) 
{
  const char* destination = ais_msg.get_destination();
  return destination;
}

const char * getName(AIS& ais_msg) 
{
  const char* name = ais_msg.get_shipname();
  return name;
}

long showMMSI(AIS& ais_msg) {
  unsigned long mmsi = ais_msg.get_mmsi();
  return mmsi;
}

long showIMO(AIS& ais_msg) {
  unsigned long imo = ais_msg.get_imo();
  return imo;
}

unsigned int getShipType(AIS& ais_msg)
{
  unsigned int type = ais_msg.get_shiptype();
  return type;
}

String getDraught(AIS& ais_msg)
{
  unsigned int draught = ais_msg.get_draught();
  String integerpart = String((draught) / 10);
  String decimalpart = String((draught) % 10);
  String out = integerpart+"."+decimalpart;
  return out;
}
// ========End Static Data========

unsigned int getMSgtype(AIS& ais_msg) 
{
  unsigned int msg = ais_msg.get_numeric_type();
  return msg;
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
    // ais = Serial2.readStringUntil('\n');
  }
  ais.trim();
  return ais;
}

char * converttoChars(String sentence)
{
  char * buf = (char *) malloc (500);
  sentence.toCharArray(buf,sentence.length());
  
  return buf;
}

char * getDynamicMessageA(char * rawAIS)
{
  char * ais = rawAIS;
  char getChannel = ais[12];
  int store_index = 13;
  if(getChannel == ',')
  {
    store_index = 13;
  }
  else
  {
    store_index = 12;
  }
  static char out[1001];
  memset(out, '\0', 1001); // clear char buffer
  int length = loA(ais);
  int indexGPS = String(ais).indexOf('$');

  if(indexGPS>0)
  {
    for (int i = 0; i < indexGPS-14; i++)
    {
      char a_s = ais[store_index];
      out[i] = a_s;
      store_index++;
    }
  }
  else
  {
    for (int i = 0; i < length; i++)
    {
      char a_s = ais[store_index];
      out[i] = a_s;
      store_index++;
    }
  }
  return out;
}

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  delay(3000);
}

void loop() { //Choose Serial1 or Serial2 as required
  // Serial.print(getSentence());
  char * rawAIS = converttoChars(getSentence());
  char * msg = getDynamicMessageA(rawAIS);
  // // Serial.println(msgType);
  AIS ais_msg(msg);
  // Dynamic
  lon = showLongitude(ais_msg) ;
  lat = showLatitude(ais_msg);
  sog = showSOG(ais_msg);
  cog = showCOG(ais_msg);
  hdg = heading(ais_msg);
  timeS = timestamp(ais_msg);
  // Static
  long mmsi = showMMSI(ais_msg);
  long imo = showIMO(ais_msg);
  String eta = getETA(ais_msg);
  destination = getDestination(ais_msg);
  name = getName(ais_msg);
  String draught = getDraught(ais_msg);
  int shiptype = getShipType(ais_msg);

  Serial.println(lon);
  Serial.println(lat);
  Serial.println(sog);
  Serial.println(cog);
  Serial.println(hdg);
  Serial.println(timeS);
  Serial.println(eta);
  Serial.println(destination);
  Serial.println(name);
  Serial.println(imo);
  Serial.println(mmsi);
  Serial.println(draught);
  Serial.println(shiptype);
  Serial.println(getMSgtype(ais_msg));
  
  free(rawAIS);
  delay(100);
}
