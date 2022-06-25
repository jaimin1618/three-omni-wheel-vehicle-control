#include<SoftwareSerial.h>
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

#define username "nodemcu"
#define password "abc123456"
#define Rx D6
#define Tx D7
SoftwareSerial UART(Rx, Tx);

ESP8266WebServer server(80);

String msg = "";

void rootHandler() {
  msg = server.arg("pressed_key");
  server.send(200, "text/plain", "");

  for (int i = 0; i < 10; ++i) {
    sendingData(msg);
    delay(10);
  }
}

void R1Location() {
  String x = server.arg("POSITION_X");
  String y = server.arg("POSITION_Y");

  msg = "<" + x + "," + y + ">";
  server.send(200, "text/plain", "");

  for (int i = 0; i < 10; ++i) {
    sendingData(msg);
    delay(10);
  }
}

void R1Speed() {
  msg = server.arg("PWM");
  server.send(200, "text/plain", "");

  String b_speed = "[" + msg.substring(0, 3) + "]";
  UART.write(b_speed.c_str());
  delay(10);
}

void setup() {
  UART.begin(74880);
  Serial.begin(74880);

  WiFi.softAP(username, password);
  server.on("/R1/", rootHandler);
  server.on("/R1/location/", R1Location);
  server.on("/R1/PWM", R1Speed);

  server.begin();
}

void sendingData(String msg) {
  for (int i = 0; i < msg.length(); ++i) {
    UART.write(msg.c_str()[i]);
  }
}

void loop() {
  server.handleClient();
  // delay(10);

  Serial.print("current key: ");
  Serial.print(msg.c_str());
  Serial.println();
  // sendingData(msg);

}
