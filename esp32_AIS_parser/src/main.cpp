#include <Arduino.h>
#include <AIS.h>

/* Coming from util.h */

#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )


void printDegrees(long min4) 
{
  //Serial.print(min4); Serial.print(" (");Serial.print(min4,16);Serial.println(")");
  long intPart = min4 / 60L;
  long fracPart = intPart % 10000L;
  if (fracPart < 0)
    fracPart = -fracPart;
  char frac[6];
  sprintf(frac, "%04ld", fracPart);
  Serial.print(intPart/10000L);Serial.print(".");Serial.print(frac);
}

void getNumeric(AIS& ais_msg){
  unsigned int numericType = ais_msg.get_numeric_type();
  Serial.print("Message ID : "); Serial.println(numericType);
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

void showTimeStamp(AIS& ais_msg){
  unsigned int TimeStamp = ais_msg.get_timeStamp();
  Serial.print("Returned Time Stamp: "); Serial.println(TimeStamp);  
}

void ShowPositionAccurarcy(AIS& ais_msg){
  bool posAcc = ais_msg.get_posAccuracy_flag();
  Serial.print("Returned Position Accuracy: "); Serial.println(posAcc);  
}

void ShowShipType(AIS& ais_msg){
  unsigned int ShipType = ais_msg.get_shiptype();
  Serial.print("Returned ship type "); Serial.println(ShipType);  
}

void ShowShipName(AIS& ais_msg){
  Serial.print("Returned Ship Name : "); Serial.println(ais_msg.get_shipname());
}

void setup() {
  Serial.begin(115200);
  //AIS ais_msg("58wt8Ui`g??r21`7S=:22058<v05Htp000000015>8OA;0sk,0*7B \n eQ8823mDm3kP00000000000,2*5D");
  AIS ais_msg("13u?etPv2;0n:dDPwUM1U1Cb069D,0*23");
  //AIS ais_msg("14eG;o@034o8sd<L9i:a;WF>062D");
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
  getNumeric(ais_msg);
  showMMSI(ais_msg);
  showSOG(ais_msg);
  showCOG(ais_msg);
  showLatitude(ais_msg);
  showLongitude(ais_msg);
  showTimeStamp(ais_msg);
  ShowPositionAccurarcy(ais_msg);
  ShowShipType(ais_msg);
  ShowShipName(ais_msg);
}

void loop() {

}