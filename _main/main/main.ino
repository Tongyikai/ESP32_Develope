#include <Adafruit_GFX.h> // 通用圖形庫，提供了在各種顯示設備上繪製基本圖形元素
#include <Adafruit_SSD1306.h> // 特定硬體驅動庫，專門用於控制基於 SSD1306 控制器的 OLED 顯示器

#include <LiquidCrystal_I2C.h> //控制基於 I2C 介面轉接板的 LCD 字符顯示器的庫

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Keypad.h>


// 螢幕控制 OLED, LCD
Adafruit_SSD1306 oled(128, 64, &Wire, -1); // OLED
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD, 使用掃描到的地址 0x27
// hd44780_I2Cexp lcd; // LCD，hd44780 會自動偵測 I2C 地址 <-- 新增這一行
bool displayOn_Screen = false;


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


// 腳位定義（根據你實際接線）
const int soilAnalogPin = 35;    // GPIO5 接類比輸出 A0
// const int soilPin = 5;       // 土壤感測器 D0 腳位（數位訊號）
const int relayPin = 26;      // 控制繼電器的腳位

// 閾值設定（根據實測值調整）
const int dryThreshold = 2000;  // 小於此值視為乾燥


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

void yButton() {
  // LCD 顯示土壤參數+ 背光開啟
  int soilValue = analogRead(soilAnalogPin);
  lcd.backlight();          // ✅ 開背光
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("analogRead");
  lcd.setCursor(0,1);
  lcd.print(String(" ") + soilValue + " < " + dryThreshold);

  digitalWrite(relayPin, LOW); // 啟動馬達（視你的模組邏輯，如需反向改為 LOW）
  delay(10000);                  // 澆水 5 秒
  digitalWrite(relayPin, HIGH);  // 關閉馬達
  Serial.println("Done.");
}

void hButton() {

}


/* ******************************** setup ******************************** */
void setup() {
  Serial.begin(115200);
  delay(2000); // 等待啟動穩定
  Serial.println("********** ESP32 Ready **********");
  pinMode(ledPin, OUTPUT);

  // 自動澆水系統soilAnalogPin
  // pinMode(soilPin, INPUT);     // 感測器輸出為數位訊號
  pinMode(soilAnalogPin, INPUT);  // 類比
  pinMode(relayPin, OUTPUT);   // 繼電器為輸出腳
  digitalWrite(relayPin, HIGH); // 預設不啟動馬達（LOW 為關閉）根據你繼電器模組的邏輯需要反過來（部分模組 LOW 為啟動）


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


  // 啟動AP, 名稱與密碼
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
    if (key == 'Y') {
      yButton();
    }
    if (key == 'H') {
      hButton();
    }
  }

  
  // 自動澆水系統
  int soilValue = analogRead(soilAnalogPin);
  Serial.print("Soil value: ");
  Serial.println(soilValue);

  if (dryThreshold <  soilValue) {
    Serial.println("Soil is dry → watering...");

    //LCD 顯示土壤參數+ 背光開啟
    lcd.backlight();          // ✅ 開背光
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("analogRead");
    lcd.setCursor(0,1);
    lcd.print(String(" ") + dryThreshold + " < " +  soilValue);

    digitalWrite(relayPin, LOW);   // 啟動水泵
    delay(5000);                    // 澆水 5 秒
    digitalWrite(relayPin, HIGH);    // 關閉水泵
  } else {
    Serial.println("Soil is wet → no action.");
    digitalWrite(relayPin, HIGH);    // 保持關閉
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil is wet → no action.");
  }
  


  /*
  int soilState = digitalRead(soilPin);

  Serial.print("Soil dry? ");
  Serial.println(soilState == LOW ? "Yes" : "No");

  if (soilState == LOW) {  // FC-28 感測器乾燥時輸出 LOW
    Serial.println("Watering now...");
    digitalWrite(relayPin, HIGH); // 啟動馬達（視你的模組邏輯，如需反向改為 LOW）
    delay(5000);                  // 澆水 5 秒
    digitalWrite(relayPin, LOW);  // 關閉馬達
    Serial.println("Done.");
  }
  delay(2000); // 每 2 秒偵測一次
  */

}
