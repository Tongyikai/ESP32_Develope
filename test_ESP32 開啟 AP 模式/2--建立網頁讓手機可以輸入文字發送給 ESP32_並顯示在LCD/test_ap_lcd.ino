#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h> 
// 建立一個簡單的網頁讓手機可以輸入文字發送給 ESP32, 並顯示在LCD
// LCD 1602
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

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
    <title>ESP32 Send text</title>
  </head>
  <body>
    <h2>Send text to ESP32</h2>
    <form action="/send" method="GET">
      <input type="text" name="msg" placeholder="Enter text">
      <input type="submit" value="Send">
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
      Serial.println("Message received:" + userMsg);
      server.send(200, "text/html", "<h3>Received:" + userMsg + "</h3><a href='/'>return</a>");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Received:");
      lcd.setCursor(0,1);
      lcd.print(userMsg);
    } else {
      server.send(200, "text/html", "<h3>No messages received</h3><a href='/'>return</a>");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No messages received");
      lcd.setCursor(0,1);
      lcd.print("None!");
    }
  });

  server.begin();

  // === LCD 1602 ===
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("** ESP32 single chip **");
  lcd.setCursor(0,1);
  lcd.print("Launch!");

  // LED 腳位設定
  pinMode(LED_BUILTIN, OUTPUT);  // <<== 這一行是讓 LED 能正常使用

  Serial.println("ESP32 IS READY！");
}

void loop() {
  server.handleClient();
}
