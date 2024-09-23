#include <String.h>
#include <SPI.h>
#include <Ethernet.h>
#include <AIS.h>

#define SCK       12
#define SS        10
#define MOSI      11
#define MISO      13
#define SPI_FREQ  32000000

// === GOLBAL VARIABLE DECLARATION ===========
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
int msgid;
// ================================================ 

// Mac Address + ip Address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Server ADDRESS
char server[] = "projects-demo.atspace.cc";  

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
EthernetClient client;

// ====== AIS Data Decoder / Retriver Function ======
String showLongitude(AIS& ais_msg);
String showLatitude(AIS& ais_msg);
String showSOG(AIS& ais_msg);
String showCOG(AIS& ais_msg);
unsigned int heading(AIS& ais_msg);
unsigned int timestamp(AIS& ais_msg);
String getETA(AIS& ais_msg);
const char * getDestination(AIS& ais_msg);
const char * getName(AIS& ais_msg);
long showMMSI(AIS& ais_msg);
long showIMO(AIS& ais_msg);
unsigned int getShipType(AIS& ais_msg);
String getDraught(AIS& ais_msg);
unsigned int getMSgtype(AIS& ais_msg);

// ===== Webclient routine ===== 
void SendRequest(String AISDataString);


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
  delay(1000);
}
// ============== END OF SETUP ====================///


 // =============== MAIN LOOP ===================== //
void loop(){
   // ============= AIS Data Read ============ 
  // input AIS Messages
  AIS ais_msg("58wt8Ui`g??r21`7S=:22058<v05Htp000000015>8OA;0sk,0*7B");

  // Dynamic
  msgid =  getMSgtype(ais_msg);
  lon = showLongitude(ais_msg) ;
  lat = showLatitude(ais_msg);
  sog = showSOG(ais_msg);
  cog = showCOG(ais_msg);
  hdg = heading(ais_msg);
  timeS = timestamp(ais_msg);
  // Static
  mmsi = showMMSI(ais_msg);
  imo = showIMO(ais_msg);
  eta = getETA(ais_msg);
  destination = getDestination(ais_msg);
  name = getName(ais_msg);
  String draught = getDraught(ais_msg);
  shiptype = getShipType(ais_msg);

  /*
  Serial.println("============== AIS Decoded Data ==============");
  Serial.println(mmsi);
  Serial.println(msgid);
  Serial.println(lon);
  Serial.println(lat);
  Serial.println(sog);
  Serial.println(cog);
  Serial.println(hdg);
  Serial.println(time);
  Serial.println(eta);
  Serial.println(destination);
  Serial.println(shiptype);
  Serial.println(name);
  Serial.println(imo);
  Serial.println(draught);*/

  SendRequest();
}
// =============== END OF MAIN LOOP ================= //








