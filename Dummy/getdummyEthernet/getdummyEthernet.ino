#include <AIS.h>
#include <SPI.h>
#include <Ethernet.h>

#define SCK       12
#define SS        10
#define MOSI      11
#define MISO      13
#define SPI_FREQ  32000000
#define RXD2 18
#define TXD2 17
/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 100); // example IPAddress ip(192, 168, 1, 100);

// Init server name
char server[] = "aispangkalbalam.42web.io";
const int port = 80;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

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
  if(Serial1.available()>0) {
    ais = Serial1.readStringUntil('\n');
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
  Serial1.begin(38400, SERIAL_8N1, RXD2, TXD2);
  // // start the Ethernet connection:
  // SPI.begin(SCK, MISO, MOSI, SS);
  // SPI.setFrequency(SPI_FREQ);
  // // You can use Ethernet.init(pin) to configure the CS pin
  // Ethernet.init(SS);  // Most Arduino shields
  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("Failed to configure Ethernet using DHCP");
  //   // try to congifure using IP address instead of DHCP:
  //   Ethernet.begin(mac, ip);
  // }
  // else
  // {
  //   Serial.println("Connected");
  // }
  delay(1000);
}

void loop() { //Choose Serial1 or Serial2 as required
  // Serial.print(getSentence());
  char * rawAIS = converttoChars(getSentence());
  // char * msg = getDynamicMessageA(rawAIS);
  // // Serial.println(msgType);
  Serial.println(rawAIS);
  //Serial.println(msg);
  // AIS ais_msg(msg);
  // // Dynamic
  // String lon = showLongitude(ais_msg) ;
  // String lat = showLatitude(ais_msg);
  // String sog = showSOG(ais_msg);
  // String cog = showCOG(ais_msg);
  // unsigned int hdg = heading(ais_msg);
  // unsigned int time = timestamp(ais_msg);
  // // Static
  // long mmsi = showMMSI(ais_msg);
  // long imo = showIMO(ais_msg);
  // String eta = getETA(ais_msg);
  // String destination = getDestination(ais_msg);
  // String name = getName(ais_msg);
  // String draught = getDraught(ais_msg);
  // int shiptype = getShipType(ais_msg);
 
  // Serial.println(lon);
  // Serial.println(lat);
  // Serial.println(sog);
  // Serial.println(cog);
  // Serial.println(hdg);
  // Serial.println(time);
  // Serial.println(eta);
  // Serial.println(destination);
  // Serial.println(name);
  // Serial.println(imo);
  // Serial.println(mmsi);
  // Serial.println(draught);
  // Serial.println(shiptype);
  // Serial.println(getMSgtype(ais_msg));
  // if (client.connect(server, port)) { 
  //   Serial.print("connected asksensors.com");
  //   //Create a URL for the request
  //   String url = "http://asksensors.com/api.asksensors/write/";
  //   url += apiKeyIn;
  //   url += "?module1=";
  //   url += dumData;
  //   Serial.print("********** requesting URL: ");
  //   Serial.println(url);
  //   //Make a HTTP request:
  //   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  //             "Host: " + server + "\r\n" +
  //             "Connection: close\r\n\r\n");
  //   client.println();
    
  //   Serial.println("> Request sent to ASKSENSORS");
  // } else {
  //   // if you didn't get a connection to the server:
  //   Serial.println("connection failed");
  // }
  
  free(rawAIS);
  delay(10);
}