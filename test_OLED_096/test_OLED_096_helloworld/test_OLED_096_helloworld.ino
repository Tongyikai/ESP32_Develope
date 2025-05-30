#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 模擬電壓
float voltage = 25.0;

void setup() {
  Wire.begin(21, 22); // 如果你用的是 D21/SDA, D22/SCL
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 根據你的模組地址
  display.clearDisplay();

  display.setTextSize(1);      // 文字大小
  display.setTextColor(SSD1306_WHITE); 
}

void loop() {
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Host: A");

  display.setCursor(0, 16);
  display.println("IP: 192.168.4.1");

  display.setCursor(0, 32);
  display.print("V: ");
  display.print(voltage, 1); // 顯示一位小數

  display.display();

  // 模擬電壓改變（可移除）
  voltage += 0.1;
  if (voltage > 26) voltage = 25.0;

  delay(1000); // 每秒更新一次
}
