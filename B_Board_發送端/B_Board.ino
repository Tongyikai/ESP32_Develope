#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h> 

// 填入你的接收端 MAC 地址
uint8_t receiverAddress[] = {0x14, 0x33, 0x5c, 0x0d, 0xdd, 0xdc}; 

// LCD 1602
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// 發送訊息, 含次數計數器
typedef struct struct_message {
  int count;          // 加一個計數器欄位
  char msg[32];
} struct_message;

struct_message myData;
int sendCount = 0;     // 本地發送次數計數器

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW 初始化失敗！");
    return;
  }

  // === 新版的 Peer 設定方式 ===
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("加 Peer 失敗！");
    return;
  }

  // === LCD 1602 ===
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("setup!");
  // lcd.setCursor(0,1);
  // lcd.print("Init!");

  // LED 腳位設定
  pinMode(LED_BUILTIN, OUTPUT);  // <<== 這一行是讓 LED 能正常使用

  Serial.println("準備開始發送訊息！");
}

void loop() {
  // === LED ===
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(5000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second

  // strcpy(myData.msg, "I'm Site B");
  // esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));
  // Serial.println("訊息已發送！");
  // delay(5000);

  // 含次數計數器
  sendCount++;
  myData.count = sendCount;
  sprintf(myData.msg, "Hello! No.%d", sendCount);

  esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));
  Serial.printf("第 %d 次訊息已發送：%s\n", sendCount, myData.msg);

  delay(60000);

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Message Sent!");
  lcd.setCursor(0,1);
  lcd.print(sendCount);
}
