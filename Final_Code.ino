#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>
#include <WiFiManager.h>

TM1637Display display = TM1637Display(D2, D3);
WiFiServer server(80);
const long utcOffsetInSeconds = 19800;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);
  display.setBrightness(2);
  display.clear();
  display.showNumberDec(0, false);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Time");  // AP Name for configuration portal
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  timeClient.begin();
  pinMode(D5, OUTPUT);
  digitalWrite(D5, HIGH);
  pinMode(D6, OUTPUT);
  digitalWrite(D6, HIGH);
  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);
  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);
  Serial.print("Connecting To :");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  server.begin();
  delay(2000);
}

void loop() {
  int A, B;
  timeClient.update();
  
  A = timeClient.getHours() * 100 + timeClient.getMinutes();
  B = timeClient.getSeconds();
  if ((B % 2) == 0) {
    display.showNumberDecEx(A, 0b01000000, false, 4, 0);
  } else {
    display.showNumberDecEx(A, 0b00000000, false, 4, 0);
  }
  WiFiClient client;
  client = server.available();
  if (client == 1) {
    String request = client.readStringUntil('\n');
    client.flush();
    if (request.indexOf("D51") != -1) {
      digitalWrite(D5, LOW);
    } else if (request.indexOf("D50") != -1) {
      digitalWrite(D5, HIGH);
    } else if (request.indexOf("D61") != -1) {
      digitalWrite(D6, LOW);
    } else if (request.indexOf("D60") != -1) {
      digitalWrite(D6, HIGH);
    } else if (request.indexOf("D71") != -1) {
      digitalWrite(D7, LOW);
    } else if (request.indexOf("D70") != -1) {
      digitalWrite(D7, HIGH);
    } else if (request.indexOf("D81") != -1) {
      digitalWrite(D8, LOW);
    } else if (request.indexOf("D80") != -1) {
      digitalWrite(D8, HIGH);
    } else {
      Serial.println("Wrong Option! Error\n");
    }
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type : text/html");
  client.println("");
  client.println("<!DOCTYPE html>");
  client.println("<html lang=\"en\">");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("<title>pks Demo</title>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div>");
  client.println("<h1>This project is created By Praween Soni.</h1>");
  client.println("<div style=\"margin: 35px;\">");
  client.println("<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D51\">Led ON 1</a>&nbsp;&nbsp;&nbsp;<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D50\">Led OFF 1</a>");
  client.println("</div>");
  client.println("<div style=\"margin: 35px;\">");
  client.println("<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D61\">Led ON 2</a>&nbsp;&nbsp;&nbsp;<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D60\">Led OFF 2</a>");
  client.println("</div>");
  client.println("<div style=\"margin: 35px;\">");
  client.println("<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D71\">Led ON 3</a>&nbsp;&nbsp;&nbsp;<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D70\">Led OFF 3</a>");
  client.println("</div>");
  client.println("<div style=\"margin: 35px;\">");
  client.println("<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D81\">Led ON 4</a>&nbsp;&nbsp;&nbsp;<a style=\"border: 2px solid black; padding: 10px; background-color: aqua;\" href=\"?/D80\">Led OFF 4</a>");
  client.println("</div>");
  client.println("</div>");
  client.println("</body></html>");
}
