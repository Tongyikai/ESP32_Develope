#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 分壓設定
const float R1 = 30000.0;  // 上電阻 30k
const float R2 = 22000.0;  // 下電阻 22k
const int analogPin = 34;  // 接 ADC 腳位（只能讀不能輸出）

void setup() {
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  analogReadResolution(12); // 0~4095
}

void loop() {
  int raw = analogRead(analogPin);
  float adcVoltage = raw * (3.3 / 4095.0);
  float batteryVoltage = adcVoltage * ((R1 + R2) / R2);

  // 顯示文字
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Battery: ");
  display.print(batteryVoltage, 2);
  display.println(" V");

  // 電池 icon 框架
  int x = 20;
  int y = 20;
  int w = 40;
  int h = 20;
  display.drawRect(x, y, w, h, SSD1306_WHITE);         // 外框
  display.drawRect(x + w, y + h / 3, 4, h / 3, SSD1306_WHITE); // 電池頭

  // 根據電壓計算格數
  int level = 0;
  if (batteryVoltage >= 8.5) level = 5;
  else if (batteryVoltage >= 8.0) level = 4;
  else if (batteryVoltage >= 7.5) level = 3;
  else if (batteryVoltage >= 7.0) level = 2;
  else if (batteryVoltage >= 6.5) level = 1;
  else level = 0;

  // 畫電量格子（每格 6px 寬）
  for (int i = 0; i < level; i++) {
    display.fillRect(x + 2 + i * 7, y + 2, 5, h - 4, SSD1306_WHITE);
  }

  // 若電壓過低，顯示警告
  if (batteryVoltage < 6.5) {
    display.setCursor(0, 50);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("Warning: LOW!");
  }

  display.display();
  delay(1000);
}
