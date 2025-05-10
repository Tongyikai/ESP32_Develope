const unsigned char cat_face [] PROGMEM = {
  0b00010000, 0b00001000,
  0b00111000, 0b00011100,
  0b01111100, 0b00111110,
  0b11111110, 0b01111111,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b10110111, 0b11101101,
  0b10111111, 0b11111101,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b11100111, 0b11100111,
  0b01111111, 0b11111110,
  0b00111111, 0b11111100,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000
};

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 圖示大小
const int iconWidth = 16;
const int iconHeight = 16;

int x = 0;
int dx = 1;

void setup() {
  Wire.begin(21, 22); // SDA, SCL
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  display.clearDisplay();

  // 繪製圖示
  display.drawBitmap(x, 24, cat_face, iconWidth, iconHeight, SSD1306_WHITE);

  display.display();

  // 移動邏輯
  x += dx;
  if (x < 0 || x > SCREEN_WIDTH - iconWidth) {
    dx = -dx; // 碰邊反彈
  }

  delay(30); // 控制速度
}
