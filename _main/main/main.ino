#include <Adafruit_GFX.h> // 通用圖形庫，提供了在各種顯示設備上繪製基本圖形元素
#include <Adafruit_SSD1306.h> // 特定硬體驅動庫，專門用於控制基於 SSD1306 控制器的 OLED 顯示器

#include <LiquidCrystal_I2C.h> //控制基於 I2C 介面轉接板的 LCD 字符顯示器的庫
// #include <hd44780.h>       // 新增 hd44780 核心函式庫
// #include <hd44780_I2Cexp.h> // 新增 hd44780 的 I2C 擴展板支援

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Keypad.h>


// 螢幕控制 OLED, LCD
Adafruit_SSD1306 oled(128, 64, &Wire, -1); // OLED
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD, 使用掃描到的地址 0x27
// hd44780_I2Cexp lcd; // LCD，hd44780 會自動偵測 I2C 地址 <-- 新增這一行
bool displayOn_Screen = false;
//bool lastButtonState = HIGH;


// 矩陣鍵盤
const byte ROWS = 2;
const byte COLS = 2;
char keys[ROWS][COLS] = {
  {'O','X'},
  {'Y','H'}
};
byte rowPins[ROWS] = {32, 33};
byte colPins[COLS] = {27, 14}; // ⚠️ GPIO12 改成 GPIO13
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// AP 模式
const char* ssid = "ESP32-LED-AP";
const char* password = "55779900";
bool apModelOn = false;
WebServer server(80);
const int ledPin = 2;


// 檔案上傳
void listFiles() {
  Serial.println("LittleFS 檔案列表：");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.println(file.name());
    file = root.openNextFile();
  }
}


// 矩陣鍵盤
void oButton() {
  displayOn_Screen = !displayOn_Screen;

  if (displayOn_Screen) {
    Serial.println("顯示：開啟");

    // OLED 顯示
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.println("OLED running");
    oled.display();

    // LCD 顯示 + 背光開啟
    lcd.backlight();          // ✅ 開背光
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LCD running");
  } else {
    Serial.println("顯示：關閉");

    oled.clearDisplay();
    oled.display();

    lcd.clear();              // 清空內容
    lcd.noBacklight();        // ✅ 關背光
  }

  delay(300);
  Serial.println("螢幕開啟或關閉");

//  lastButtonState = buttonState;
}
void xButton() {
    apModelOn = !apModelOn;

  if (apModelOn) {
    Serial.println("AP：開啟");
    WiFi.mode(WIFI_AP); // 明確設定為 AP 模式
    bool result = WiFi.softAP(ssid, password); // 啟動 AP

    if (result) {
      Serial.println("✅ AP 啟動成功");
      Serial.print("IP Address: ");
      Serial.println(WiFi.softAPIP());

      
      oled.clearDisplay(); // OLED 顯示文字
      oled.setTextSize(1);
      oled.setTextColor(SSD1306_WHITE);
      oled.setCursor(0, 0);
      oled.println("IP: ");
      // oled.print(batteryVoltage, 2);
      oled.setCursor(0, 16);
      oled.println("192.168.4.1");
      oled.display();

      
      lcd.clear(); // LCD 顯示文字
      lcd.setCursor(0,0);
      lcd.print("AP Model: available");
      lcd.setCursor(0,1);
      lcd.print("### OK! ###");
    } else {
      Serial.println("❌ AP 啟動失敗");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AP Model: disabled");
      lcd.setCursor(0,1);
      lcd.print("### failed ###");
    }
    delay(1000); // 給系統多點時間來啟動 WiFi

  } else {
    WiFi.softAPdisconnect(true);  // 關閉 AP
    Serial.println("AP：關閉");

    oled.clearDisplay(); // OLED 顯示文字
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("AP: ");
    // oled.print(batteryVoltage, 2);
    oled.setCursor(0, 16);
    oled.println("disable");
    oled.display();


    lcd.clear(); // LCD 顯示文字
    lcd.setCursor(0,0);
    lcd.print("AP Model: disable");
  }
}


void setup() {
  Serial.begin(115200);
  delay(2000); // 等待啟動穩定
  Serial.println("********** ESP32 Ready **********");
  pinMode(ledPin, OUTPUT);


  // OLED 初始化
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("找不到 OLED");
  }
  oled.clearDisplay();
  oled.display();

  //LCD 初始化
  lcd.init();         // 必須有
  lcd.backlight();    // 開啟背光
  lcd.clear();        // 清除顯示
  lcd.setCursor(0,0);
  lcd.print("bootstrap:#####");
  lcd.setCursor(0,1);
  lcd.print("#####");

  // LCD 初始化
  // hd44780 的 begin() 會自動偵測 I2C 地址和 LCD 尺寸
  // if (lcd.begin(16, 2)) { // 16 列 2 行
  //   Serial.println("LCD 初始化成功");
  //   lcd.backlight(); // 開啟背光
  //   lcd.clear();     // 清除顯示
  // } else {
  //   Serial.println("LCD 初始化失敗！請檢查接線和 I2C 地址。");
  //   // 如果 LCD 初始化失敗，可以在這裡加入一些錯誤處理
  // }

  /*
  // 啟動AP, 名稱與密碼
  WiFi.mode(WIFI_AP); // 明確設定為 AP 模式
  WiFi.softAP(ssid, password, 6); // 嘗試明確指定頻道，例如頻道 6
  delay(100); // 增加延遲
  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());
  */

  WiFi.mode(WIFI_AP); // 明確設定為 AP 模式
  bool result = WiFi.softAP(ssid, password); // 啟動 AP

  if (result) {
    Serial.println("✅ AP 啟動成功");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    oled.clearDisplay(); // OLED 顯示文字
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.println("IP: ");
    // oled.print(batteryVoltage, 2);
    oled.setCursor(0, 16);
    oled.println("192.168.4.1");
    oled.display();
  } else {
    Serial.println("❌ AP 啟動失敗");
  }
  delay(1000); // 給系統多點時間來啟動 WiFi


  // 檔案讀取
  if (!LittleFS.begin()) {
    Serial.println("LittleFS 初始化失敗，但繼續執行");
    // return; ← 拿掉這個
  }
  listFiles();


  // 網頁
  server.on("/", HTTP_GET, []() {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
      server.send(500, "text/plain; charset=utf-8", "Not found index.html");
      return;
    }
    server.streamFile(file, "text/html; charset=utf-8");
    file.close();
  });
  server.serveStatic("/style.css", LittleFS, "/style.css");

  server.on("/ledOn", []() {
    if (server.hasArg("duration")) {
      int duration = server.arg("duration").toInt();
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
      // server.send(200, "text/html", "<h3>LED Turn On</h3><a href='/'>GO BACK</a>");
      server.send_P(200, "text/html", PSTR("<h3>LED Turn On</h3><a href='/'>GO BACK</a>"));

      delay(duration * 1000);
      // millis(duration * 1000);
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    } else {
      server.send(400, "text/html", "<h3>Missing duration parameter</h3><a href='/'>GO BACK</a>");
    }
  });

  server.on("/ledOff", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/html", "<h3>LED Turn Off</h3><a href='/'>GO BACK</a>");
  });

  server.begin();
}


void loop() {
  server.handleClient();
  
  // 矩陣鍵盤
  char key = keypad.getKey();
  if (key) {
    Serial.print("Pressed: ");
    Serial.println(key);
    if (key == 'X') {
      xButton();
    }
    if (key =='O') {
      oButton();
    }
  }
}
