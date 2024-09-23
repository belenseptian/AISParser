// ============ SERVER REQUEST FUNCTION ============= //
void SendRequest(){

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    //client.println("GET /temp.php?q=arduino HTTP/1.1");
    // Make a HTTP request:
    String request = "GET /temp.php?Message_ID=" + String(msgid) + "&MMSI=" + String(mmsi) +
                      "&Longitude=" + lon + "&Latitude=" + lat + "&SoG=" + sog + "&CoG=" +
                      cog + "&Heading=" + String(hdg) + "&Time_stamp=" + String(timeS) + "&Draught="
                      + draught + "&ETA=" + eta + "&IMO=" + imo + "&Ship_type=" + String(shiptype)
                      + "&Ship_name=" + String(name) + "&Destination=" + String(destination) + " HTTP/1.1";
    client.println(request);
    client.println("Host: projects-demo.atspace.cc");
    client.println("Connection: close");
    client.println();
    Serial.println(request);
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection to server failed");
  }

    // if there are incoming bytes available
  // from the server, read them and print them:
  /*if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }*/
}
// ================ END OF SERVER REQUEST FUNCTION ================ //