#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  analogReadResolution(12); // 12-bit (0~4095)
}

void loop() {

  // 顯示在 OLED 上
  // display.clearDisplay();
  // display.setCursor(0, 10);
  // display.print("Battery: ");
  // display.print(batteryVoltage, 2);
  // display.println(" V");
  // display.display();

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("0");
  display.println("0");

  display.setCursor(0, 1);
  display.print("1");
  display.println("1");

  display.setCursor(0, 2);
  display.print("2");
  display.println("2");

  display.setCursor(0, 3);
  display.print("3");
  display.println("3");

  display.display();

  delay(1000);
}
