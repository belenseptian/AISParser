#include <Arduino.h>
#include <AIS.h>

/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )

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

String getDestination(AIS& ais_msg) 
{
  const char* destination = ais_msg.get_destination();
  return String(destination);
}

String getName(AIS& ais_msg) 
{
  const char* name = ais_msg.get_shipname();
  return String(name);
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

void setup() {
  Serial.begin(115200);

  AIS ais_msg("13u?etPv2;0n:dDPwUM1U1Cb069D,0*23");
  /* Console output (!AIVDM,1,1,,A,14eG;o@034o8sd<L9i:a;WF>062D,0*7D)
   *  
   * Returned MMSI: 316001245 (12D5CBDD )
   * Returned SOG: 19.6 nm
   * Returned COG: 235.0 degrees
   * Returned LAT: 49.2002 degrees
   * Returned LONG: -123.8777 degrees
   */
  //AIS ais_msg("177KQJ5000G?tO`K>RA1wUbN0TKH");
  /*  Console output: (!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C)
   *   
   *  Returned MMSI: 477553000 (1C76E168 )
   *  Returned SOG: 0.0 nm
   *  Returned COG: 51.0 degrees
   *  Returned LAT: 47.5828 degrees
   *  Returned LONG: -122.3458 degrees
   */
  // Dynamic
  String lon = showLongitude(ais_msg) ;
  String lat = showLatitude(ais_msg);
  String sog = showSOG(ais_msg);
  String cog = showCOG(ais_msg);
  unsigned int hdg = heading(ais_msg);
  unsigned int time = timestamp(ais_msg);
  // Static
  long mmsi = showMMSI(ais_msg);
  long imo = showIMO(ais_msg);
  String eta = getETA(ais_msg);
  String destination = getDestination(ais_msg);
  String name = getName(ais_msg);
  String draught = getDraught(ais_msg);
  int shiptype = getShipType(ais_msg);
 
  Serial.println(lon);
  Serial.println(lat);
  Serial.println(sog);
  Serial.println(cog);
  Serial.println(hdg);
  Serial.println(time);
  Serial.println(eta);
  Serial.println(destination);
  Serial.println(name);
  Serial.println(imo);
  Serial.println(mmsi);
  Serial.println(draught);
  Serial.println(shiptype);
  Serial.println(getMSgtype(ais_msg));

  delay(100);
}

void loop() {

}
