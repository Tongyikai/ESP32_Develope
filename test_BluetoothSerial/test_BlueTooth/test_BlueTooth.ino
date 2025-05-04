#include <BluetoothSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 1602
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
BluetoothSerial SerialBT;

String receivedData = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("ESP32_LCD"); //Bluetooth device name
  SerialBT.println("The device started, now you can pair it with bluetooth!");

  // 初始化 LCD
  lcd.init();         // 初始化 LCD
  lcd.backlight();    // 開啟背光
  lcd.setCursor(0, 0);
  lcd.print("Waiting BT...");

  // LED 腳位設定
  pinMode(LED_BUILTIN, OUTPUT);  // <<== 這一行是讓 LED 能正常使用

  Serial.println("ESP32 IS READY！");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();

      if (incomingChar == '\n' || incomingChar == '\r') {
        if (receivedData.length() > 0) { // ✅ 加這行避免處理空資料

          // 格式化並顯示字串 (常駐顯示最新傳來內容)
          displayOnLCD(receivedData);
          // 回傳確認訊息給手機
          SerialBT.println("ESP32已收到：" + receivedData);
          // 清空 buffer
          receivedData = ""; // 清空準備接下一段
        }
        // 換行符號代表一筆訊息結束，更新 LCD 顯示
        // lcd.clear();
        // lcd.setCursor(0, 0);
        // lcd.print(receivedData.substring(0, 16));
        // if (receivedData.length() > 16) {
        //   lcd.setCursor(0, 1);
        //   lcd.print(receivedData.substring(16, 32));
        // }

      } else {
        receivedData += incomingChar;
        if (receivedData.length() > 32) { // 最多兩行
          receivedData = receivedData.substring(0, 32);
        }
      }
  }
  
  // delay(20);

  // === LED ===
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
}

void displayOnLCD(String text) {
  // 固定覆蓋顯示，防止殘影
  lcd.setCursor(0, 0);
  lcd.print("                "); // 清第一行
  lcd.setCursor(0, 1);
  lcd.print("                "); // 清第二行

  lcd.setCursor(0, 0);
  lcd.print(text.substring(0, 16));
  if (text.length() > 16) {
    lcd.setCursor(0, 1);
    lcd.print(text.substring(16, 32));
  }
}
