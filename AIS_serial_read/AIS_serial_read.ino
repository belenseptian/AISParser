char data_in;

#define RX 18
#define TX 17

void setup() {
  Serial.begin(115200);
  Serial1.begin(38400,SERIAL_8N1,RX,TX);

}

void loop() {
  while(Serial1.available()) {
    data_in = Serial1.read();// read the incoming data as string
    Serial.print(data_in);
  }
}
