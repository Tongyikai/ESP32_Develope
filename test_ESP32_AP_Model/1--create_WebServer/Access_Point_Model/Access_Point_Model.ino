#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

// ESP32 Web Server, 並在網頁設定 LED 燈 開啟關閉
// 將 HTML 和 CSS 分離成外部檔案，並在 ESP32 啟動時從 SPIFFS（SPI Flash File System）或 LittleFS（較新的替代方案）中讀取這些檔案。

const char* ssid = "ESP32-LED-AP";
const char* password = "55779900";
WebServer server(80);
const int ledPin = 2;


void listFiles() {
  Serial.println("LittleFS 檔案列表：");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.println(file.name());
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  // 初始化 LittleFS
  if (!LittleFS.begin()) {
    Serial.println("LittleFS 初始化失敗");
    return;
  }

   listFiles();

  // 設定根目錄 "/"
  server.on("/", HTTP_GET, []() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain; charset=utf-8", "找不到 index.html");
    return;
  }
  server.streamFile(file, "text/html; charset=utf-8");
  file.close();
  });

  // 靜態檔案（CSS）
  server.serveStatic("/style.css", LittleFS, "/style.css");

  server.on("/ledOn", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/html", "<h3>LED 已開啟</h3><a href='/'>返回</a>");
  });

  server.on("/ledOff", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/html", "<h3>LED 已關閉</h3><a href='/'>返回</a>");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
