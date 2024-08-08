// for receiver

#include <SPI.h>
#include <mcp2515_can.h>
const int SPI_CS_PIN = 9;
mcp2515_can CAN_RECEIVE(SPI_CS_PIN); // Set CS pin
void setup() {
  Serial.begin(9600);
  while (CAN_OK != CAN_RECEIVE.begin(CAN_500KBPS)) { // init can bus : baudrate = 500k
    Serial.println("CAN_RECEIVE init fail, retry...");
    delay(100);
  }
  Serial.println("CAN_RECEIVE init ok!");
}


void loop() {
  unsigned char len = 0;
  unsigned char buf[8];

  if (CAN_MSGAVAIL == CAN_RECEIVE.checkReceive()) { // Check for received data
    CAN_RECEIVE.readMsgBuf(&len, buf); // Read data: len = data length, buf = data byte(s)
    // Print message ID and length
    Serial.print("ID: ");
    Serial.print(CAN_RECEIVE.getCanId(), HEX);
    Serial.print("  Data Length: ");
    Serial.println(len);
    // Print each byte of the data
    // Serial.print("  Frame: ");
    // for (int i = 0; i < len; i++) {
    //   Serial.print(buf[i], HEX);
    //   Serial.print("\t");
    // }
    float receivedSpeed;
    memcpy(&receivedSpeed, buf, sizeof(receivedSpeed));

    Serial.println(receivedSpeed);

    Serial.println();
    Serial.println("-------------------------------");
  }
}