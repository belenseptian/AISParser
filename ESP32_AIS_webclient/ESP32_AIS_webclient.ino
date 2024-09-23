#include <String.h>
#include <SPI.h>
#include <Ethernet.h>
#include <AIS.h>

#define SCK       12
#define SS        10
#define MOSI      11
#define MISO      13
#define SPI_FREQ  32000000

/* Coming from util.h */
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                  ((x)<< 8 & 0x00FF0000UL) | \
                  ((x)>> 8 & 0x0000FF00UL) | \
                  ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )

// Mac Address + ip Address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Server ADDRESS
char server[] = "aispangkalbalam.42web.io/temp.php";   

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// ====== AIS Data Decoder / Retriver Function ======
String showLongitude(AIS& ais_msg);
String showLatitude(AIS& ais_msg);
String showSOG(AIS& ais_msg);
String showCOG(AIS& ais_msg);
unsigned int heading(AIS& ais_msg);
unsigned int timestamp(AIS& ais_msg);
String getETA(AIS& ais_msg);
String getDestination(AIS& ais_msg);
String getName(AIS& ais_msg);
long showMMSI(AIS& ais_msg);
long showIMO(AIS& ais_msg);
unsigned int getShipType(AIS& ais_msg);
String getDraught(AIS& ais_msg);
unsigned int getMSgtype(AIS& ais_msg);

// ===== Webclient routine ===== 
void SendRequest(char * AISDataString);


//======================== SETUP ================== //
void setup() {
  SPI.begin(SCK, MISO, MOSI, SS);
  SPI.setFrequency(SPI_FREQ);
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(SS);  // Most Arduino shields
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
    // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}
// ============== END OF SETUP ====================///


 // =============== MAIN LOOP ===================== //
void loop()
{
   // ============= AIS Data Read ============ 
  // input AIS Messages
  AIS ais_msg("58wt8Ui`g??r21`7S=:22058<v05Htp000000015>8OA;0sk,0*7B");

  // Dynamic
  unsigned int msgtype =  getMSgtype(ais_msg);
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


  String Decoded_AIS_data_string = "MESSAGE ID : " + String(msgtype) + "\nMMSI : " 
                        + String(mmsi) + "\nLONGITUDE : " + lon + 
                        "\nLATITUDE : " + lat + "\nSOG : "+ sog +"\nCOG : " 
                        + cog + "\nHEADING : " + String(hdg) + "\nTIMESTAMP : " 
                        + String(time) + "\nIMO : " + String(imo) +
                        "\nETA :" + eta + "\nDESTINATION : " + destination + 
                        "\nDRAUGHT : " + draught + "\nSHIP TYPE : " + String(shiptype) 
                        + "\nSHIP NAME : " + name + "\0";
  
  Serial.println("===AIS Data===");
  Serial.println(Decoded_AIS_data_string);
  SendRequest(Decoded_AIS_data_string);

  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

}
// =============== END OF MAIN LOOP ================= //


// ============ SERVER REQUEST FUNCTION ============= //
void SendRequest(char * AISDataString){

  char outbuf[200];

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    sprintf(outbuf,"GET /temp.php?data=%s ", String(AISDataString));
    client.println(outbuf);
    client.println("Host: aispangkalbalam.42web.io");
    client.println("Connection: close");
    client.println();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}
// ================ END OF SERVER REQUEST FUNCTION ================ //


// ================== AIS DATA RETRIVER FUNCTION ================= //
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

// ============== END OF AIS DATA RETRIVER =================



