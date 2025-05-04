#include <WiFi.h>
#include <WebServer.h>
// 建立一個簡單的網頁讓手機可以輸入文字發送給 ESP32

// 設定 AP 名稱和密碼
const char* ssid = "ESP32-AP";
const char* password = "55779900";

// 建立 WebServer 實例（監聽80埠）
WebServer server(80);

// HTML 表單頁面
const char* htmlForm = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title>ESP32 傳送文字</title>
  </head>
  <body>
    <h2>傳送文字到 ESP32</h2>
    <form action="/send" method="GET">
      <input type="text" name="msg" placeholder="輸入文字">
      <input type="submit" value="送出">
    </form>
  </body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // 啟動 AP 模式
  WiFi.softAP(ssid, password);

  // 顯示 AP 模式的 IP（手機要連這個 IP）
  Serial.println("Access Point Started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  // 預設是 192.168.4.1

   // 處理根目錄（顯示表單）
  server.on("/", []() {
    server.send(200, "text/html", htmlForm);
  });

  // 處理表單送出資料
  server.on("/send", []() {
    if (server.hasArg("msg")) {
      String userMsg = server.arg("msg");
      Serial.println("收到的訊息：" + userMsg);
      server.send(200, "text/html", "<h3>已收到：" + userMsg + "</h3><a href='/'>返回</a>");
    } else {
      server.send(200, "text/html", "<h3>未收到任何訊息</h3><a href='/'>返回</a>");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
