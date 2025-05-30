#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN 25

Adafruit_SSD1306 oled(128, 64, &Wire, -1);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 使用掃描到的地址 0x27

bool displayOn = false;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // OLED 初始化
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("找不到 OLED");
  }
  oled.clearDisplay();
  oled.display();

  // LCD 初始化
  lcd.init();         // 必須有
  lcd.backlight();    // 開啟背光
  lcd.clear();        // 清除顯示
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    displayOn = !displayOn;

    if (displayOn) {
      Serial.println("顯示：開啟");

      // OLED 顯示
      oled.clearDisplay();
      oled.setTextSize(1);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 0);
      oled.println("OLED 顯示中");
      oled.display();

      // LCD 顯示 + 背光開啟
      lcd.backlight();          // ✅ 開背光
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("LCD 顯示中");
    } else {
      Serial.println("顯示：關閉");

      oled.clearDisplay();
      oled.display();

      lcd.clear();              // 清空內容
      lcd.noBacklight();        // ✅ 關背光
    }

    delay(300);
  }

  lastButtonState = buttonState;
}
