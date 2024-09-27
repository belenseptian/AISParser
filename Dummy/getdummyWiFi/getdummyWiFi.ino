/* Libraries */
#include <AIS.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define RXD2 16
#define TXD2 17

const char * destination;
const char *name; 
char *rawAIS;
String lon, lat, sog, cog, eta, draught, gps;
unsigned int hdg, timeS, msgType;
long mmsi, imo;
int shiptype;

/* Communication */
// const char *ssid =  "anomali";           // replace with your wifi ssid and wpa2 key
// const char *pass =  "Lifeischoice*123";
const char *ssid =  "robot";           // replace with your wifi ssid and wpa2 key
const char *pass =  "umdprobot";
String serverName = "http://202.10.40.122:8080/api/sentences";

WiFiClient client;

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
    // ais = Serial2.readStringUntil('!');
    ais = Serial2.readStringUntil('\n');
  }
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

void initWifi()
{
  delay(10);
  // Serial.println("Connecting to ");
  // Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("");
  // Serial.println("WiFi connected");
  delay(2000);
}

void sendtoServer(char * nmea)
{
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
                           
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String httpRequestData = "sentence="+String(nmea);           
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    
    // if (httpResponseCode>0) {
    //   Serial.print("HTTP Response code: ");
    //   Serial.println(httpResponseCode);
    //   String payload = http.getString();
    //   Serial.println(payload);
    // }
    // else {
    //   Serial.print("Error code: ");
    //   Serial.println(httpResponseCode);
    // }
    // Free resources
    http.end();
  }
  else {
    // Serial.println("WiFi Disconnected");
  }  
}

void taskTransmit(void *pvParameters)
{
  while (1)
  {
    if(gps.equals("!") || gps.equals("$"))
    {
      sendtoServer(rawAIS);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void taskRead(void *pvParameters)
{
  while (1)
  {
    rawAIS = converttoChars(getSentence());
    gps = String(rawAIS[0]);

    free(rawAIS);
    delay(5);
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

void initTask()
{
  xTaskCreatePinnedToCore(
    taskTransmit, // Function name of the task
    "Task 1",       // Name of the task (e.g. for debugging)
    10000,          // Stack size (bytes)
    NULL,           // Parameter to pass
    1,              // Task priority
    NULL,           // Task handle
    0               // run on Core 0
  );

  xTaskCreatePinnedToCore(
    taskRead,  // Function name of the task
    "Task 2", // Name of the task (e.g. for debugging)
    10000,    // Stack size (bytes)
    NULL,     // Parameter to pass
    1,        // Task priority
    NULL,     // Task handle
    1         // run on Core 1
  );
}

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  initTask();
  delay(1000);
  initWifi();
  delay(3000);
}

void loop() {
}