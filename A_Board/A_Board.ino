#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// LCD 1602
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// 發送訊息, 印出收到次數
typedef struct struct_message {
  int count;          // 對應發送端的計數欄位
  char msg[32];
} struct_message;

struct_message incomingData;
int receiveCount = 0;

// 修改 callback 函式：參數型態要改
void onReceiveData(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  receiveCount++;
  Serial.printf("收到第 %d 次訊息（傳送端編號 %d）：%s\n", 
                receiveCount, incomingData.count, incomingData.msg);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // STA模式

    if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW 初始化失敗！");
    return;
  }

  esp_now_register_recv_cb(onReceiveData);  // 註冊正確型態的 callback

  // === LCD 1602 ===
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("setup! ");
  lcd.setCursor(0,1);
  lcd.print("0");

  // LED 腳位設定
  pinMode(LED_BUILTIN, OUTPUT);  // <<== 這一行是讓 LED 能正常使用

}

void loop() {
  // === LED ===
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Receiving: ");
  lcd.setCursor(0,1);
  lcd.print(incomingData.msg);    
}