#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-LED-AP";
const char* password = "55779900";

WebServer server(80);
const int ledPin = 2;  // 預設是板上的內建 LED（GPIO2）

// HTML 控制頁面
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 CONTROL LED</title>
</head>
<body>
  <h2>LED CONTROL PANEL</h2>
  <form action="/ledOn" method="GET">
    <button type="submit">TURN ON 💡</button>
  </form>
  <form action="/ledOff" method="GET">
    <button type="submit">TURN OFF 📴</button>
  </form>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // 啟動 AP 模式
  WiFi.softAP(ssid, password);
  Serial.println("AP Started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // 處理網頁
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  // 開燈
  server.on("/ledOn", []() {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
    server.send(200, "text/html", "<h3>LED ON</h3><a href='/'>GO BACK</a>");
  });

  // 關燈
  server.on("/ledOff", []() {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
    server.send(200, "text/html", "<h3>LED OFF</h3><a href='/'>GO BACK</a>");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
